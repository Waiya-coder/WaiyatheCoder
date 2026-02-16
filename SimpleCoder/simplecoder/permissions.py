"""Permission management for file operations."""

from typing import Set
from rich.prompt import Confirm


class PermissionManager:
    """Manages user permissions for file operations."""
    
    def __init__(self, session_persist: bool = False):
        """
        Initialize permission manager.
        
        Args:
            session_persist: If True, permissions persist for the session
        """
        self.session_persist = session_persist
        self.granted_permissions: Set[str] = set()
        
    def request_permission(self, action: str, target: str) -> bool:
        """
        Request permission from user for an action.
        
        Args:
            action: The action to perform (e.g., 'read', 'write', 'delete')
            target: The target of the action (e.g., file path)
            
        Returns:
            True if permission granted, False otherwise
        """
        # Check if already granted for this session
        permission_key = f"{action}:{target}"
        if self.session_persist and permission_key in self.granted_permissions:
            return True
        
        # Ask user
        prompt = f"Allow {action} on '{target}'?"
        granted = Confirm.ask(prompt, default=False)
        
        # Store if session persist enabled
        if granted and self.session_persist:
            self.granted_permissions.add(permission_key)
            
        return granted
    
    def clear_permissions(self):
        """Clear all granted permissions."""
        self.granted_permissions.clear()