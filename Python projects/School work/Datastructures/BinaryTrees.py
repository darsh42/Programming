class Node:
    def __init__(self,node):
        self.node = node
        self.left = None
        self.right = None
        self.print = False
 
    def addNode(self, node):
        if node > self.node:
            if bool(self.right):
                self.right = Node.addNode(node)
            else:
                self.right = Node(node)
        
        if node < self.node:
            if bool(self.left):
                self.left.addNode(node)
            else:
                self.left = Node(node)

    def display(self):
        if bool(self.left) and not(self.left.print):
            self.left.display()
        else:
            print(self.left)
            if bool(self.right):
                self.print = True
                print(self.right)
                self.node.display()

            else:
                self.print = True
                self.node.display()

def main():
    tree = Node(5)
    tree.addNode(4)
    tree.addNode(6)
    tree.display()
main()