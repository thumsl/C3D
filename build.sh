# !/bin/bash

gcc game.c window.c render.c utils.c -o game.o -lSDL2 -lGL -lGLEW -lm
