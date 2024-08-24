import numpy as np

class Stack:
    def __init__(self, Size) -> None:
        self.size = Size
        self.isFull = False
        self.isEmpty = True
        self.header = 0
        self.tail = 0
        self.data = []

    def checkIsFull(self):
        if len(self.data) == self.size:
            self.isFull = True
        else:
            return

    def addItem(self, items): 
        for item in items:
            self.checkIsFull()

            if self.isFull:
                return
            
            elif self.isEmpty:
                self.data.append(item)
                self.isEmpty = False               

            else:          
                self.data.append(item)
                self.tail += 1

    def removeItem(self):
        self.data.pop(self.tail)
        self.tail -= 1


class Queue(Stack):
    def __init__(self, Size) -> None:
        super().__init__(Size)

    def removeItem(self):
        del self.data[self.header]
        self.header += 1








def main():
    stack = Stack(3)
    stack.addItem([3])
    stack.addItem([4])
    print(f"Header:{stack.header}\nTail:{stack.tail}")
    stack.removeItem()
    print(stack.data)
    
    queue = Queue(3)
    queue.addItem([3,4,5,6,7,8])
    queue.removeItem()
    print(queue.data)

main()
