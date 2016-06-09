# !/bin/bash

gcc game.c window.c input.c render.c -o game.o -lSDL2 -lGL -lGLEW
