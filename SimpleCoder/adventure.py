class Room:
    def __init__(self, name, description):
        self.name = name
        self.description = description
        self.exits = {}

    def link_room(self, direction, room):
        self.exits[direction] = room

    def get_details(self):
        print(f"\nLocation: {self.name}")
        print(self.description)
        print("Exits: " + ", ".join(self.exits.keys()))

# Create rooms
entrance = Room("Entrance", "A cold, stone foyer with a heavy oak door.")
hallway = Room("Hallway", "A long, dimly lit corridor lined with portraits.")
treasure_room = Room("Treasure Room", "A glittering chamber filled with gold and jewels.")

# Connect rooms
entrance.link_room("north", hallway)
hallway.link_room("south", entrance)
hallway.link_room("north", treasure_room)
treasure_room.link_room("south", hallway)

# Simple navigation test
current_room = entrance
while True:
    current_room.get_details()
    command = input("\nWhere do you want to go? (or 'quit'): ").lower()
    
    if command == "quit":
        break
    elif command in current_room.exits:
        current_room = current_room.exits[command]
    else:
        print("You can't go that way.")
