import numpy as np




class LinkedList:
    def __init__(self, length) -> None:
        self.length = length # Length of data list
        
        self.list = np.zeros((self.length,2)) # Data list
        self.free = np.zeros((self.length)) # List of free values 
        
        self.next_free = 0 #creates list of free values then returns the first one
        self.start = -1 # start index
        self.previous_node = -1 #refrence to previous node

    def find_next_free (self):
        try:
            self.next_free = np.where(self.free == 0)[0][0]
        except ValueError:
            print("list is full")

    def Node(self,data):
        return (data,self.previous_node)

    def add_node(self,data):
        self.list[self.next_free] = self.Node(data)
        self.previous_node = self.next_free
        self.free[self.next_free] = 1
        self.find_next_free()

    def del_node(self):
        for item in self.list:
            print (self.list[np.where(self.list == item)[0][0],1])

    
    def display(self):
        print(self.list)

def main():
    #l = LinkedList(4)
    #l.add_node(23)
    #l.add_node(34)
    #l.add_node(67)
    #l.del_node()


main()
