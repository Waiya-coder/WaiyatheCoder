class Room:
    def __init__(self, name, description):
        self.name = name
        self.description = description
        self.exits = {}

    def connect(self, direction, room):
        self.exits[direction] = room

# Create rooms
entrance = Room("Entrance", "A cold, stone entryway with torches flickering on the walls.")
hallway = Room("Hallway", "A long, narrow passage echoing with distant sounds.")
treasure_room = Room("Treasure Room", "A glittering chamber filled with gold and ancient artifacts.")

# Connect rooms
entrance.connect("north", hallway)
hallway.connect("south", entrance)
hallway.connect("north", treasure_room)
treasure_room.connect("south", hallway)

# Basic navigation test
current_room = entrance

print("Welcome to the Dungeon!")
print("Commands: 'north', 'south', 'east', 'west', 'quit'")

while True:
    print(f"\nYou are in the {current_room.name}.")
    print(current_room.description)
    print(f"Exits: {', '.join(current_room.exits.keys())}")
    
    command = input("\n> ").lower()

    if command == "quit":
        print("Thanks for playing!")
        break
    elif command in current_room.exits:
        current_room = current_room.exits[command]
    else:
        print("You can't go that way.")
