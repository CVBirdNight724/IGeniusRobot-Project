class Node():
    def __init__(self, name, variable):
        self.name = name
        self.variable = variable
        self.next = None
    def printNext(self):
        print(self.next.name)

class Linklist():
    def __init__(self):
        self.head = None
        self.tail = None
    def add(node):
        self.tail.next = node

A = Node("A", 12)
B = Node("B", 13)
C = Node("C", 14)

A.next = B
# A.printNext()

B.next = C
B.printNext()