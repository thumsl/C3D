ALL:
	gcc src/*.c examples/game.c -o bin/game.o -lGL -lGLEW -lSDL2 -lSDL2_image -lm
