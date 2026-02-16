# #!/usr/bin/env python3
# """
# Simple Text Adventure Game
# Created with SimpleCoder agent
# """

# class Room:
#     """Represents a room in the adventure."""
    
#     def __init__(self, name, description):
#         self.name = name
#         self.description = description
#         self.connections = {}
#         self.items = []
    
#     def connect(self, direction, room):
#         """Connect this room to another room."""
#         self.connections[direction] = room
    
#     def add_item(self, item):
#         """Add an item to the room."""
#         self.items.append(item)
    
#     def remove_item(self, item):
#         """Remove an item from the room."""
#         if item in self.items:
#             self.items.remove(item)
    
#     def describe(self):
#         """Describe the room."""
#         print(f"\n{self.name}")
#         print("=" * len(self.name))
#         print(self.description)
        
#         if self.items:
#             print(f"\nYou see: {', '.join(self.items)}")
        
#         if self.connections:
#             directions = ', '.join(self.connections.keys())
#             print(f"\nExits: {directions}")


# class Game:
#     """Main game controller."""
    
#     def __init__(self):
#         self.current_room = None
#         self.inventory = []
#         self.setup_world()
    
#     def setup_world(self):
#         """Create the game world."""
#         # Create rooms
#         entrance = Room("Castle Entrance", 
#                        "You stand before a grand castle. The heavy wooden doors are slightly ajar.")
        
#         hall = Room("Great Hall",
#                    "A magnificent hall with tall pillars and faded tapestries on the walls.")
        
#         library = Room("Library",
#                       "Dusty bookshelves line the walls. A single book lies open on a table.")
        
#         courtyard = Room("Courtyard",
#                         "An overgrown courtyard with a dried fountain in the center.")
        
#         # Connect rooms
#         entrance.connect("north", hall)
#         hall.connect("south", entrance)
#         hall.connect("east", library)
#         hall.connect("west", courtyard)
#         library.connect("west", hall)
#         courtyard.connect("east", hall)
        
#         # Add items
#         entrance.add_item("rusty key")
#         library.add_item("ancient tome")
#         courtyard.add_item("silver coin")
        
#         # Set starting room
#         self.current_room = entrance
    
#     def take(self, item):
#         """Take an item."""
#         if item in self.current_room.items:
#             self.current_room.remove_item(item)
#             self.inventory.append(item)
#             print(f"You take the {item}.")
#         else:
#             print(f"There is no {item} here.")
    
#     def drop(self, item):
#         """Drop an item."""
#         if item in self.inventory:
#             self.inventory.remove(item)
#             self.current_room.add_item(item)
#             print(f"You drop the {item}.")
#         else:
#             print(f"You don't have a {item}.")
    
#     def move(self, direction):
#         """Move in a direction."""
#         if direction in self.current_room.connections:
#             self.current_room = self.current_room.connections[direction]
#             self.current_room.describe()
#         else:
#             print("You can't go that way.")
    
#     def show_inventory(self):
#         """Show inventory."""
#         if self.inventory:
#             print(f"Inventory: {', '.join(self.inventory)}")
#         else:
#             print("Your inventory is empty.")
    
#     def play(self):
#         """Main game loop."""
#         print("Welcome to the Castle Adventure!")
#         print("Commands: go [direction], take [item], drop [item], inventory, look, quit")
        
#         self.current_room.describe()
        
#         while True:
#             command = input("\n> ").lower().strip()
            
#             if not command:
#                 continue
            
#             parts = command.split()
#             action = parts[0]
            
#             if action == "quit":
#                 print("Thanks for playing!")
#                 break
            
#             elif action == "look":
#                 self.current_room.describe()
            
#             elif action == "inventory" or action == "inv":
#                 self.show_inventory()
            
#             elif action == "go" and len(parts) > 1:
#                 self.move(parts[1])
            
#             elif action == "take" and len(parts) > 1:
#                 item = ' '.join(parts[1:])
#                 self.take(item)
            
#             elif action == "drop" and len(parts) > 1:
#                 item = ' '.join(parts[1:])
#                 self.drop(item)
            
#             else:
#                 print("I don't understand that command.")


# if __name__ == "__main__":
#     game = Game()
#     game.play()