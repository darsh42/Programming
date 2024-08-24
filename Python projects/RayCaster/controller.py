import pygame
import numpy

from player import Player


class Controller:
    player = None
    RESOLUTION = None

    def __init__(self, player:Player, RESOLUTION):
        self.player = player
        self.RESOLUTION = RESOLUTION

    def input_handle(self):
        keys = pygame.key.get_pressed()
        pos = self.player.position
        speed = self.player.speed

        d_pos = [0, 0]
        if keys[pygame.K_a] and pos[0] > speed:
            d_pos[0] += -speed
        if keys[pygame.K_d] and pos[0] < self.RESOLUTION[0] - speed:
            d_pos[0] += speed
        if keys[pygame.K_w] and pos[1] > speed:
            d_pos[1] += -speed
        if keys[pygame.K_s] and pos[1] < self.RESOLUTION[1] - speed:
            d_pos[1] += speed

        self.player.move(d_pos)