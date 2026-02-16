class Player:
    def __init__(self, name):
        self.name = name
        self.inventory = []

    def add_item(self, item):
        self.inventory.append(item)
        print(f"You picked up: {item}")

    def show_inventory(self):
        if not self.inventory:
            print("\nYour inventory is empty.")
        else:
            print("\nYou are carrying:")
            for item in self.inventory:
                print(f"- {item}")

class Room:
    def __init__(self, name, description):
        self.name = name
        self.description = description
        self.exits = {}
        self.item = None

    def link_room(self, room_to_link, direction):
        self.exits[direction] = room_to_link

    def get_details(self):
        print(f"\nLocation: {self.name}")
        print("--------------------")
        print(self.description)
        if self.item:
            print(f"You see a {self.item} here.")
        for direction in self.exits:
            room = self.exits[direction]
            print(f"The {room.name} is to the {direction}")

    def move(self, direction):
        if direction in self.exits:
            return self.exits[direction]
        else:
            print("You can't go that way!")
            return self

def main():
    player = Player("Adventurer")
    
    # Create rooms
    kitchen = Room("Kitchen", "A room with a delicious smell of fresh bread.")
    hall = Room("Hallway", "A long, dimly lit hallway.")
    ballroom = Room("Ballroom", "A grand room with a shiny wooden floor.")

    # Add items to rooms
    kitchen.item = "Knife"
    hall.item = "Key"
    ballroom.item = "Trophy"

    # Link rooms
    kitchen.link_room(hall, "south")
    hall.link_room(kitchen, "north")
    hall.link_room(ballroom, "east")
    ballroom.link_room(hall, "west")

    # Set starting room
    current_room = kitchen

    print("Welcome to the Mansion Adventure!")
    print("Commands: 'north', 'south', 'east', 'west', 'take', 'inventory', 'quit'")

    while True:
        current_room.get_details()
        command = input("\n> ").lower()

        if command == "quit":
            print("Thanks for playing!")
            break
        elif command in ["north", "south", "east", "west"]:
            current_room = current_room.move(command)
        elif command == "take":
            if current_room.item:
                player.add_item(current_room.item)
                current_room.item = None
            else:
                print("There is nothing here to take.")
        elif command == "inventory":
            player.show_inventory()
        else:
            print("I don't understand that command.")

if __name__ == "__main__":
    main()
