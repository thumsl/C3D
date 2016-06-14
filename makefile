all:
	gcc src/*.c -lGL -lGLEW -lSDL2 -lSDL2_image -lm -o bin/game.o
