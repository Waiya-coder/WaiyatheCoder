"""Task planning and decomposition."""

from typing import List, Dict, Any


class Task:
    """Represents a subtask."""
    
    def __init__(self, description: str, dependencies: List[int] = None):
        self.description = description
        self.dependencies = dependencies or []
        self.completed = False


class TaskPlanner:
    """Plans and tracks task decomposition."""
    
    def __init__(self):
        self.tasks: List[Task] = []
        self.current_task_index = 0
    
    def create_plan(self, llm_call_fn, main_task: str) -> List[Task]:
        """
        Create a task plan by asking LLM to decompose the task.
        
        Args:
            llm_call_fn: Function to call LLM
            main_task: The main task description
        
        Returns:
            List of subtasks
        """
        prompt = f"""Break down this task into 3-5 concrete subtasks:

Task: {main_task}

Respond with a numbered list of subtasks, one per line. Each subtask should be actionable and specific.
Example format:
1. Create project directory structure
2. Write main.py with basic setup
3. Add error handling
"""
        
        try:
            response = llm_call_fn(prompt)
            
            # Parse response into tasks
            self.tasks = self._parse_tasks(response)
            
            return self.tasks
        except Exception as e:
            # Fallback: treat as single task
            self.tasks = [Task(main_task)]
            return self.tasks
    
    def _parse_tasks(self, response: str) -> List[Task]:
        """Parse LLM response into task list."""
        tasks = []
        
        lines = response.strip().split('\n')
        for line in lines:
            line = line.strip()
            
            # Skip empty lines
            if not line:
                continue
            
            # Remove numbering (e.g., "1.", "2)", etc.)
            if line[0].isdigit():
                line = line.split('.', 1)[-1].split(')', 1)[-1].strip()
            
            # Remove markdown bullets
            if line.startswith('-') or line.startswith('*'):
                line = line[1:].strip()
            
            if line:
                tasks.append(Task(line))
        
        # If parsing failed, return original as single task
        if not tasks:
            tasks = [Task(response)]
        
        return tasks
    
    def get_next_task(self) -> Task | None:
        """Get the next incomplete task."""
        for i in range(self.current_task_index, len(self.tasks)):
            task = self.tasks[i]
            
            # Check if dependencies are met
            deps_met = all(
                self.tasks[dep].completed 
                for dep in task.dependencies 
                if dep < len(self.tasks)
            )
            
            if not task.completed and deps_met:
                self.current_task_index = i
                return task
        
        return None
    
    def mark_completed(self, task: Task):
        """Mark a task as completed."""
        task.completed = True
    
    def get_progress(self) -> str:
        """Get progress summary."""
        completed = sum(1 for t in self.tasks if t.completed)
        total = len(self.tasks)
        
        if total == 0:
            return "No tasks"
        
        status_lines = []
        for i, task in enumerate(self.tasks, 1):
            status = "✓" if task.completed else "○"
            status_lines.append(f"{status} {i}. {task.description}")
        
        progress = f"Progress: {completed}/{total} tasks completed\n"
        progress += "\n".join(status_lines)
        
        return progress
    
    def is_complete(self) -> bool:
        """Check if all tasks are completed."""
        return all(task.completed for task in self.tasks)