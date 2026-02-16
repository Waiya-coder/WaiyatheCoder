"""RAG for code search using AST-based chunking."""

import ast
from pathlib import Path
from typing import List, Dict, Any, Optional
import numpy as np


class CodeChunk:
    """Represents a chunk of code (function, class, etc.)."""
    
    def __init__(self, code: str, chunk_type: str, name: str, file_path: str):
        self.code = code
        self.chunk_type = chunk_type  # 'function', 'class', 'module'
        self.name = name
        self.file_path = file_path
        self.embedding: Optional[np.ndarray] = None


class CodeRAG:
    """RAG system for semantic code search."""
    
    def __init__(self, embedder_model: str = "gemini/gemini-embedding-001"):
        """Initialize RAG with embedding model."""
        self.embedder_model = embedder_model
        self.chunks: List[CodeChunk] = []
        self.embedder = None
        
        # Try to initialize embedder
        try:
            from litellm import embedding
            self.embedder = embedding
        except:
            pass
    
    def index_codebase(self, pattern: str = "**/*.py", root_dir: Path = None):
        """Index codebase by chunking files with AST."""
        if root_dir is None:
            root_dir = Path.cwd()
        
        files = list(root_dir.glob(pattern))
        
        for file_path in files:
            try:
                self._index_file(file_path)
            except Exception as e:
                print(f"Error indexing {file_path}: {e}")
        
        # Generate embeddings for all chunks
        if self.embedder:
            self._generate_embeddings()
    
    def _index_file(self, file_path: Path):
        """Index a single Python file using AST."""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                source = f.read()
            
            tree = ast.parse(source)
            
            # Extract functions and classes
            for node in ast.walk(tree):
                if isinstance(node, ast.FunctionDef):
                    chunk_code = ast.get_source_segment(source, node)
                    if chunk_code:
                        chunk = CodeChunk(
                            code=chunk_code,
                            chunk_type="function",
                            name=node.name,
                            file_path=str(file_path)
                        )
                        self.chunks.append(chunk)
                
                elif isinstance(node, ast.ClassDef):
                    chunk_code = ast.get_source_segment(source, node)
                    if chunk_code:
                        chunk = CodeChunk(
                            code=chunk_code,
                            chunk_type="class",
                            name=node.name,
                            file_path=str(file_path)
                        )
                        self.chunks.append(chunk)
            
            # Also add full module as a chunk
            module_chunk = CodeChunk(
                code=source[:1000],  # First 1000 chars
                chunk_type="module",
                name=file_path.stem,
                file_path=str(file_path)
            )
            self.chunks.append(module_chunk)
            
        except Exception as e:
            # If AST parsing fails, fall back to simple chunking
            with open(file_path, 'r', encoding='utf-8') as f:
                source = f.read()
            
            chunk = CodeChunk(
                code=source[:1000],
                chunk_type="module",
                name=file_path.stem,
                file_path=str(file_path)
            )
            self.chunks.append(chunk)
    
    def _generate_embeddings(self):
        """Generate embeddings for all chunks."""
        if not self.embedder:
            return
        
        for chunk in self.chunks:
            try:
                # Create text representation
                text = f"{chunk.chunk_type} {chunk.name}:\n{chunk.code[:500]}"
                
                # Get embedding
                response = self.embedder(
                    model=self.embedder_model,
                    input=[text]
                )
                
                # Extract embedding vector
                if response and response.get('data'):
                    chunk.embedding = np.array(response['data'][0]['embedding'])
            except Exception as e:
                print(f"Error embedding chunk {chunk.name}: {e}")
    
    def search(self, query: str, top_k: int = 3) -> List[CodeChunk]:
        """Search for relevant code chunks."""
        if not self.chunks:
            return []
        
        # If no embedder, fall back to simple text search
        if not self.embedder or not any(c.embedding is not None for c in self.chunks):
            return self._keyword_search(query, top_k)
        
        try:
            # Get query embedding
            response = self.embedder(
                model=self.embedder_model,
                input=[query]
            )
            
            query_embedding = np.array(response['data'][0]['embedding'])
            
            # Compute similarities
            similarities = []
            for chunk in self.chunks:
                if chunk.embedding is not None:
                    sim = np.dot(query_embedding, chunk.embedding) / (
                        np.linalg.norm(query_embedding) * np.linalg.norm(chunk.embedding)
                    )
                    similarities.append((chunk, sim))
            
            # Sort by similarity
            similarities.sort(key=lambda x: x[1], reverse=True)
            
            return [chunk for chunk, _ in similarities[:top_k]]
        
        except Exception as e:
            print(f"Error in semantic search: {e}")
            return self._keyword_search(query, top_k)
    
    def _keyword_search(self, query: str, top_k: int) -> List[CodeChunk]:
        """Fallback keyword-based search."""
        query_lower = query.lower()
        matches = []
        
        for chunk in self.chunks:
            if query_lower in chunk.code.lower() or query_lower in chunk.name.lower():
                matches.append(chunk)
        
        return matches[:top_k]
    
    def format_results(self, chunks: List[CodeChunk]) -> str:
        """Format search results as text."""
        if not chunks:
            return "No relevant code found."
        
        results = []
        for i, chunk in enumerate(chunks, 1):
            results.append(
                f"**Result {i}: {chunk.chunk_type} `{chunk.name}` from {chunk.file_path}**\n"
                f"```python\n{chunk.code[:300]}...\n```\n"
            )
        
        return "\n".join(results)