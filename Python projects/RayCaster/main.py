import numpy as np
import pygame

from controller import Controller
from player import Player
from map import Map

SET_RESOLUTION = (640, 480)
START_POS = (SET_RESOLUTION[0]/2, SET_RESOLUTION[1]/2)

def main():

    pygame.init()

    screen = pygame.display.set_mode([SET_RESOLUTION[0],
                                      SET_RESOLUTION[1]])

    background = pygame.Surface(SET_RESOLUTION)
    foreground = pygame.Surface(SET_RESOLUTION)

    clock = pygame.time.Clock()

    map = Map(SET_RESOLUTION, background)
    player = Player(screen, START_POS, SET_RESOLUTION, map, clock)
    controller = Controller(player, SET_RESOLUTION)

    running = True
    while running:

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False


        screen.fill((255, 255, 255))
        controller.input_handle()
        map.update(screen)
        player.update()


        pygame.display.update()
        clock.tick(60)

    pygame.quit()


if __name__ == "__main__":
    main()