CC=gcc
FLAGS=-lpulse-simple -lmad
DEBUG=-g -fsanitize=address
FILES=player.c utils/filetypes.c
BIN=player

debug:
	$(CC) $(FILES) -o $(BIN) $(FLAGS) $(DEBUG)
