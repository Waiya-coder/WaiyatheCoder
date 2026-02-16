"""Main agent with ReAct loop."""

import json
from typing import Optional
from pathlib import Path

from simplecoder.tools import FileTools
from simplecoder.rag import CodeRAG
from simplecoder.context import ContextManager
from simplecoder.planner import TaskPlanner, Task
from simplecoder.permissions import PermissionManager


class Agent:
    """SimpleCoder agent with ReAct loop."""
    
    def __init__(
        self,
        model: str = "gemini/gemini-2.0-flash-exp",
        max_iterations: int = 10,
        verbose: bool = False,
        use_planning: bool = False,
        use_rag: bool = False,
        rag_embedder: str = "gemini/gemini-embedding-001",
        rag_index_pattern: str = "**/*.py"
    ):
        """Initialize the agent."""
        self.model = model
        self.max_iterations = max_iterations
        self.verbose = verbose
        self.use_planning = use_planning
        self.use_rag = use_rag
        
        # Initialize components
        self.permission_manager = PermissionManager(session_persist=True)
        self.tools = FileTools(self.permission_manager)
        self.context = ContextManager(max_tokens=100000, keep_recent=4)
        self.planner = TaskPlanner() if use_planning else None
        
        # Initialize RAG if enabled
        self.rag: Optional[CodeRAG] = None
        if use_rag:
            self.rag = CodeRAG(embedder_model=rag_embedder)
            if self.verbose:
                print(f"Indexing codebase with pattern: {rag_index_pattern}")
            self.rag.index_codebase(pattern=rag_index_pattern)
            if self.verbose:
                print(f"Indexed {len(self.rag.chunks)} code chunks")
        
        # Initialize LLM client
        try:
            from litellm import completion
            self.llm = completion
        except ImportError:
            raise ImportError("Please install litellm: pip install litellm")
    
    def _call_llm(self, prompt: str, system_prompt: str = None) -> str:
        """Call LLM with prompt."""
        messages = []
        
        if system_prompt:
            messages.append({"role": "system", "content": system_prompt})
        
        messages.append({"role": "user", "content": prompt})
        
        try:
            response = self.llm(
                model=self.model,
                messages=messages,
                temperature=0.7
            )
            
            return response.choices[0].message.content
        except Exception as e:
            return f"Error calling LLM: {str(e)}"
    
    def _react_step(self, task: str) -> tuple[str, bool]:
        """
        Execute one ReAct step: Thought -> Action -> Observation.
        
        Returns:
            (observation, done)
        """
        # Build system prompt
        system_prompt = self._build_system_prompt()
        
        # Build user prompt with context
        messages = self.context.get_messages()
        
        # Add current task
        if not messages or messages[-1].get("content") != task:
            self.context.add_message("user", task)
            messages = self.context.get_messages()
        
        # Call LLM with tools
        try:
            response = self.llm(
                model=self.model,
                messages=messages,
                tools=self.tools.get_tool_schemas(),
                temperature=0.7
            )
            
            message = response.choices[0].message
            
            # Check for tool calls
            if hasattr(message, 'tool_calls') and message.tool_calls:
                tool_call = message.tool_calls[0]
                tool_name = tool_call.function.name
                
                try:
                    arguments = json.loads(tool_call.function.arguments)
                except:
                    arguments = {}
                
                if self.verbose:
                    print(f"\n🔧 Tool: {tool_name}")
                    print(f"📝 Args: {arguments}")
                
                # Execute tool
                observation = self.tools.execute_tool(tool_name, arguments)
                
                if self.verbose:
                    print(f"👁️ Observation: {observation[:200]}...")
                
                # Add to context
                self.context.add_message("assistant", f"Using tool: {tool_name}")
                self.context.add_message("system", f"Tool result: {observation}")
                
                return observation, False
            
            # No tool call - final response
            response_text = message.content
            
            if self.verbose:
                print(f"\n💬 Response: {response_text[:200]}...")
            
            self.context.add_message("assistant", response_text)
            
            # Check if done
            done_keywords = ["task complete", "finished", "done", "successfully completed"]
            is_done = any(kw in response_text.lower() for kw in done_keywords)
            
            return response_text, is_done
            
        except Exception as e:
            error_msg = f"Error in ReAct step: {str(e)}"
            if self.verbose:
                print(f"\n❌ {error_msg}")
            return error_msg, False
    
    def _build_system_prompt(self) -> str:
        """Build system prompt for the agent."""
        prompt = """You are SimpleCoder, a helpful coding assistant.

You help users with coding tasks by:
- Reading and writing files
- Searching codebases
- Editing code
- Creating new projects

Available tools:
- list_files: List files in a directory
- read_file: Read file contents
- write_file: Write content to a file
- search_files: Search for files matching a pattern
- edit_file: Edit a file by replacing text

When solving a task:
1. Think through what needs to be done
2. Use tools to gather information or make changes
3. Provide clear feedback on progress
4. Signal completion clearly when done

Be concise and helpful."""
        
        return prompt
    
    def run(self, task: str) -> str:
        """
        Run the agent on a task.
        
        Args:
            task: The task description
            
        Returns:
            Final response
        """
        # Clear context for new task
        self.context.clear()
        
        # Add system prompt
        system_prompt = self._build_system_prompt()
        self.context.add_message("system", system_prompt)
        
        # Handle RAG query if enabled
        if self.use_rag and self.rag and ("what" in task.lower() or "where" in task.lower() or "find" in task.lower()):
            if self.verbose:
                print("\n🔍 Searching codebase...")
            
            results = self.rag.search(task, top_k=3)
            rag_context = self.rag.format_results(results)
            
            # Add RAG results to context
            self.context.add_message("system", f"Relevant code:\n{rag_context}")
        
        # Handle planning if enabled
        if self.use_planning and self.planner:
            if self.verbose:
                print("\n📋 Creating task plan...")
            
            self.planner.create_plan(
                lambda p: self._call_llm(p, system_prompt),
                task
            )
            
            if self.verbose:
                print(self.planner.get_progress())
            
            # Execute tasks sequentially
            responses = []
            while not self.planner.is_complete():
                next_task = self.planner.get_next_task()
                if not next_task:
                    break
                
                if self.verbose:
                    print(f"\n▶️ Executing: {next_task.description}")
                
                # Run ReAct loop for this subtask
                response = self._run_react_loop(next_task.description)
                responses.append(response)
                
                # Mark completed
                self.planner.mark_completed(next_task)
                
                if self.verbose:
                    print(self.planner.get_progress())
            
            # Combine responses
            final_response = "\n\n".join(responses)
            final_response += f"\n\n{self.planner.get_progress()}"
            
            return final_response
        else:
            # No planning - just run ReAct loop
            return self._run_react_loop(task)
    
    def _run_react_loop(self, task: str) -> str:
        """Run ReAct loop for a task."""
        final_response = ""
        
        for iteration in range(self.max_iterations):
            if self.verbose:
                print(f"\n--- Iteration {iteration + 1}/{self.max_iterations} ---")
            
            observation, done = self._react_step(task)
            final_response = observation
            
            if done:
                if self.verbose:
                    print("\n✅ Task completed")
                break
        
        return final_response