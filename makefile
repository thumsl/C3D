ALL:
	gcc src/*.c -o bin/game.o -lGL -lGLEW -lSDL2 -lSDL2_image -lm $1
