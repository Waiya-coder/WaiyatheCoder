"""Context management with token tracking and compacting."""

from typing import List, Dict, Any
import tiktoken


class ContextManager:
    """Manages conversation context with token limits."""
    
    def __init__(self, max_tokens: int = 100000, keep_recent: int = 3):
        """
        Initialize context manager.
        
        Args:
            max_tokens: Maximum context tokens before summarization
            keep_recent: Number of recent messages to keep intact
        """
        self.max_tokens = max_tokens
        self.keep_recent = keep_recent
        self.messages: List[Dict[str, str]] = []
        
        # Use cl100k_base encoding (GPT-4 / GPT-3.5-turbo)
        try:
            self.encoding = tiktoken.get_encoding("cl100k_base")
        except:
            # Fallback: approximate token count
            self.encoding = None
    
    def count_tokens(self, text: str) -> int:
        """Count tokens in text."""
        if self.encoding:
            return len(self.encoding.encode(text))
        else:
            # Rough approximation: 1 token ≈ 4 characters
            return len(text) // 4
    
    def get_total_tokens(self) -> int:
        """Get total tokens in context."""
        total = 0
        for msg in self.messages:
            total += self.count_tokens(msg.get("content", ""))
        return total
    
    def add_message(self, role: str, content: str):
        """Add a message to context."""
        self.messages.append({"role": role, "content": content})
    
    def get_messages(self) -> List[Dict[str, str]]:
        """Get all messages, compacting if needed."""
        if self.get_total_tokens() > self.max_tokens:
            self._compact_context()
        return self.messages
    
    def _compact_context(self):
        """Compact context by summarizing old messages."""
        if len(self.messages) <= self.keep_recent:
            return
        
        # Split into old and recent
        old_messages = self.messages[:-self.keep_recent]
        recent_messages = self.messages[-self.keep_recent:]
        
        # Create summary of old messages
        summary_parts = []
        for msg in old_messages:
            role = msg.get("role", "unknown")
            content = msg.get("content", "")[:200]  # First 200 chars
            summary_parts.append(f"[{role}]: {content}...")
        
        summary = "\n".join(summary_parts)
        summary_msg = {
            "role": "system",
            "content": f"[Previous conversation summary]\n{summary}"
        }
        
        # Replace old messages with summary
        self.messages = [summary_msg] + recent_messages
    
    def clear(self):
        """Clear all messages."""
        self.messages = []