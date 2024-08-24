import pygame

class Map:
    array_map = [
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
                 [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
                 [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
                 [1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1],
                 [1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1],
                 [1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1],
                 [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
                 [1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1],
                 [1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1],
                 [1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1],
                 [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
                 [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
                 [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
                 [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
                 [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
                 [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
                ]
    RESOLUTION = None
    surface = None

    segment_size = None

    walls = []
    
    def __init__(self, resolution, surface):
        self.surface = surface
        self.RESOLUTION = resolution

        self.segment_size = (self.RESOLUTION[0] / len(self.array_map),
                             self.RESOLUTION[1] / len(self.array_map[1]))
        
        self.draw()

    def update(self, screen):
        screen.blit(self.surface, (0,0))

    def draw(self):
        for row_num, row in enumerate(self.array_map):
            for col_num, col in enumerate(row):
                
                wall_colour = self.wall_colour(col)
                rect = pygame.Rect(row_num * self.segment_size[0],
                                   col_num * self.segment_size[1],
                                   self.segment_size[0]+1,
                                   self.segment_size[1])
                self.walls.append((rect, col))

                pygame.draw.rect(self.surface,
                                 wall_colour,
                                 rect)

    def wall_colour(self, type):
        match type:
            case 1:
                return (255,255,255)
            case _:
                return (0,0,0)


