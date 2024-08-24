#Extrnal lib
import numpy as np

#External Classes

class Pieces():
    def __init__(self):
        int self.value # K-win, Q-9, R-5, Kn-3, B-3, P-1
        bool self.team # Black-0, White-1
        self.pos # list in format (x,y), initial position of each piece

class King(Pieces):
    def __init__(self, value, team):
        super().__init__(value, team):
            self.checkmate = False
            init_pos()

    # starting position of king
    def init_pos(self, board):
        if self.team:
            board[0][4] = King(True)
        else:
            board[0][4] = King(False)

    # kings movement
    def move(self, board ):
        if self.checkmate():
            self.lost()
        else:
            if (x + 1 == self.pos[0] or x - 1 == self.pos[0]) and (y == self.pos[1]):
                self.pos[0] = x
                self.pos[1] = y

            elif (y + 1 == self.pos[1] or y - 1 == self.pos[1]) and (x == self.pos[1]):
                self.pos[0] = x
                self.pos[1] = y
            return

    # if king is taken
    def checkmate(self, possible_pos):
        for pos in possible_pos:
            if  pos[1]:
                


        
class Queen(Pieces):
    def __init__(self, value, team):
        super().__init__(value):

    def move(self, x, y):


class Bishop(Pieces):
    def __init__(self, value, team):
        super().__init__(value):

class Knight(Pieces):
    def __init__(self, value, team):
        super().__init__(value):

class Rook(Pieces):
    def __init__(self, value, team):
        super().__init__(value):


    def move(self, board):
        self.pos = board.where(self)


class Pawns(Pieces):
    def __init__(self, value, team):
        super().__init__(value):
