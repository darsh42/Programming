#External libriaries
import numpy as np

#External project files
import pieces


"""
[[[piece, is_check]][][][][][][][]]
[[][][][][][][][]]
[[][][][][][][][]]
[[][][][][][][][]]
[[][][][][][][][]]
[[][][][][][][][]]
[[][][][][][][][]]
[[][][][][][][][]]
"""

"""
[rkbqkbkr]
[pppppppp]
[________]
[________]
[________]
[________]
[________]
[________]
[pppppppp]
[rkbqkbkr]
"""

class Board():
    def __init__(self, board):
        self.board = np.zeroes((8))

    def start(self,team):
        if team:
            self.board[0] = [Rook(team), Knight(team), Bishop(team), Queen(team), King(team), Bishop(team)]


