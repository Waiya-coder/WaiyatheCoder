"""Tools for file operations."""

import os
from pathlib import Path
from typing import List, Dict, Any
from simplecoder.permissions import PermissionManager


class FileTools:
    """File operation tools for the agent."""
    
    def __init__(self, permission_manager: PermissionManager):
        """Initialize file tools with permission manager."""
        self.permission_manager = permission_manager
        self.working_dir = Path.cwd()
        
    def get_tool_schemas(self) -> List[Dict[str, Any]]:
        """Return tool schemas for LLM."""
        return [
            {
                "name": "list_files",
                "description": "List files in a directory",
                "parameters": {
                    "type": "object",
                    "properties": {
                        "path": {
                            "type": "string",
                            "description": "Directory path (default: current directory)"
                        }
                    }
                }
            },
            {
                "name": "read_file",
                "description": "Read contents of a file",
                "parameters": {
                    "type": "object",
                    "properties": {
                        "path": {
                            "type": "string",
                            "description": "File path to read"
                        }
                    },
                    "required": ["path"]
                }
            },
            {
                "name": "write_file",
                "description": "Write content to a file",
                "parameters": {
                    "type": "object",
                    "properties": {
                        "path": {
                            "type": "string",
                            "description": "File path to write"
                        },
                        "content": {
                            "type": "string",
                            "description": "Content to write"
                        }
                    },
                    "required": ["path", "content"]
                }
            },
            {
                "name": "search_files",
                "description": "Search for files matching a pattern",
                "parameters": {
                    "type": "object",
                    "properties": {
                        "pattern": {
                            "type": "string",
                            "description": "Glob pattern (e.g., '*.py', '**/*.js')"
                        }
                    },
                    "required": ["pattern"]
                }
            },
            {
                "name": "edit_file",
                "description": "Edit a file by replacing old text with new text",
                "parameters": {
                    "type": "object",
                    "properties": {
                        "path": {
                            "type": "string",
                            "description": "File path to edit"
                        },
                        "old_text": {
                            "type": "string",
                            "description": "Text to replace"
                        },
                        "new_text": {
                            "type": "string",
                            "description": "Replacement text"
                        }
                    },
                    "required": ["path", "old_text", "new_text"]
                }
            }
        ]
    
    def list_files(self, path: str = ".") -> str:
        """List files in directory."""
        try:
            target_path = Path(path)
            if not target_path.exists():
                return f"Error: Path '{path}' does not exist"
            
            if not target_path.is_dir():
                return f"Error: '{path}' is not a directory"
            
            files = []
            for item in sorted(target_path.iterdir()):
                prefix = "📁" if item.is_dir() else "📄"
                files.append(f"{prefix} {item.name}")
            
            return "\n".join(files) if files else "Empty directory"
        except Exception as e:
            return f"Error listing files: {str(e)}"
    
    def read_file(self, path: str) -> str:
        """Read file contents."""
        try:
            # Request permission
            if not self.permission_manager.request_permission("read", path):
                return "Permission denied"
            
            file_path = Path(path)
            if not file_path.exists():
                return f"Error: File '{path}' does not exist"
            
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            return f"File: {path}\n```\n{content}\n```"
        except Exception as e:
            return f"Error reading file: {str(e)}"
    
    def write_file(self, path: str, content: str) -> str:
        """Write content to file."""
        try:
            # Request permission
            action = "overwrite" if Path(path).exists() else "create"
            if not self.permission_manager.request_permission(action, path):
                return "Permission denied"
            
            file_path = Path(path)
            file_path.parent.mkdir(parents=True, exist_ok=True)
            
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(content)
            
            return f"Successfully wrote to {path}"
        except Exception as e:
            return f"Error writing file: {str(e)}"
    
    def search_files(self, pattern: str) -> str:
        """Search for files matching pattern."""
        try:
            matches = list(self.working_dir.glob(pattern))
            if not matches:
                return f"No files found matching '{pattern}'"
            
            results = [str(p.relative_to(self.working_dir)) for p in matches]
            return "\n".join(results)
        except Exception as e:
            return f"Error searching files: {str(e)}"
    
    def edit_file(self, path: str, old_text: str, new_text: str) -> str:
        """Edit file by replacing text."""
        try:
            # Request permission
            if not self.permission_manager.request_permission("edit", path):
                return "Permission denied"
            
            file_path = Path(path)
            if not file_path.exists():
                return f"Error: File '{path}' does not exist"
            
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            if old_text not in content:
                return f"Error: Text not found in file"
            
            new_content = content.replace(old_text, new_text, 1)
            
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(new_content)
            
            return f"Successfully edited {path}"
        except Exception as e:
            return f"Error editing file: {str(e)}"
    
    def execute_tool(self, tool_name: str, arguments: Dict[str, Any]) -> str:
        """Execute a tool by name."""
        tool_map = {
            "list_files": self.list_files,
            "read_file": self.read_file,
            "write_file": self.write_file,
            "search_files": self.search_files,
            "edit_file": self.edit_file
        }
        
        if tool_name not in tool_map:
            return f"Unknown tool: {tool_name}"
        
        try:
            return tool_map[tool_name](**arguments)
        except TypeError as e:
            return f"Invalid arguments for {tool_name}: {str(e)}"