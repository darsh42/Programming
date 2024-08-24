from math import hypot

class Vector:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    
    def __repr__(self):
        return "Vector(%r, %r)" % (self.x, self.y)
    
    def __abs__(self):
        return hypot(self.x, self.y)

    def __bool__(self):
        return bool(abs(self))

    def __add__(self, other):
        return Vector(self.x + other.x, 
                      self.y + other.y)

    def __mul__(self, scale):
        return Vector(self.x * scale,
                      self.y * scale)
