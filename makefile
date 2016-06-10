all:
	gcc -o game.o -lGL -lGLEW -lSDL2 -lm game.c mesh.c render.c window.c utils.c
