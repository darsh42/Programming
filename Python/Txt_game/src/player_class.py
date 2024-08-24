

class Player:
    def __init__(self, name):
        self.name = name
        self.inventory = []
        
        self.current_room = rooms["Cell"]
        self.max_inv_space = 3

    def pick_item(self, item):
        if self.inventory == self.max_inv_space:  # check if players inventory is alr    eady full
            print("You are already at maximum inventory capacity.")
            return
        for item in self.current_room["items"]:
            if item["id"] == item_id:  # if item is in the current room
                    self.inventory.append(item)  # add item to inventory
                    print("You picked up " + item["name"] + ".\n" + item["descri    ption"])  # output appropriate message
                    self.current_room["items"].remove(item)  # remove item from room
                    return
    def drop_item(item_id):
        for self.item in inventory:
            if item["id"] == item_id:  # check to see if the item passed in is in the players inventory
                current_room["items"].append(item)  # add the item dropped to the current room
                print("You dropped " + item_id + ".")  # output appropriate message
                inventory.remove(item)  # remove item from players inventory
                return
        print("You cannot drop that.")  # item is not in players inventory

