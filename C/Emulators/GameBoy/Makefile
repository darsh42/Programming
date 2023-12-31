##
# Gameboy Emulator
#
# @file
# @version 0.1

CC=gcc
files=debugger.c GBC.c cpu.c ppu.c apu.c mem.c timer.c interrupts.c sdl.c

debug:
	$(CC) src/*.c -o bin/gameboy -g -lSDL2 -lncurses -Wall -Wextra -fsanitize=address

release:
	$(CC) src/*.c -o bin/gameboy -lSDL2 -lncurses


# end
