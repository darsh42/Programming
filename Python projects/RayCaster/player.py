import numpy as np
import pygame
import math

class Player:
    surface = None
    clock = None
    map = None
    resolution = None

    FOV = 30
    ray_count = 100

    colour = (255, 255, 255)
    position = (0, 0)
    size = 5
    model_size = None
    speed = 1.33

    ray_colour = (255,0,0)
    def __init__(self, surface, start_pos, resolution, map, clock):
        self.surface = surface
        self.clock = clock
        self.map = map
        self.resolution = resolution
        self.position = start_pos
        self.set_speed()
        self.draw()
    
    def update(self):
        self.screen()
        self.draw()

    def set_speed(self):
        self.speed = self.speed
    
    
    def draw(self):
        model = pygame.Rect(self.position[0] - self.size/2.5,
                            self.position[1],
                            self.size,
                            self.size)

        self.model = pygame.draw.rect(self.surface,
                                      self.colour,
                                      model)

    def move(self, d_pos):
        tmp = tuple(map(sum, zip(self.position, d_pos)))
        if not self.collision(tmp):
            self.position = tmp

    def camera(self):
        mouse_x, mouse_y = pygame.mouse.get_pos()

        print((mouse_x, mouse_y))
        rel_x, rel_y = mouse_x - self.position[0], mouse_y - self.position[1]

        try:
            pygame.transform.rotate(self.model, math.atan(rel_x/rel_y))
        except ZeroDivisionError:
            print("Mouse on player")



    def collision(self, result_pos, walls=True):
        for sector in self.map.walls:
            if sector[0].collidepoint(result_pos) and (sector[1] != 0 or not walls):
                return True
        return False



    def screen(self):
        points = []
        angle_size = self.FOV/self.ray_count
        for i in range(self.ray_count):
            point_right = list(self.position)
            point_left = self.position
            while not self.collision(point_right):
                point_right[0]+=1
                point_right[1]+=math.atan(i*angle_size)
            
            while not self.collision(point_left, False):
                point_left[0]+=1
                point_left[1]+=math.atan(i*angle_size)
            
            points.append(point_right)
            points.append(point_left)

        print(points)
        print(self.position)
        for i in points:
            self.draw_ray(i, (0,0,255))


    def draw_ray(self, endpoint, colour):
        pygame.draw.line(self.surface,
                         colour,
                         self.position,
                         endpoint)


        
            

