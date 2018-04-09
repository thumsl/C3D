# Compiles main.c

.SUFFIXES: .c .o

CC = gcc
CFLAGS = -Wall
LIBS = -lGL -lGLEW -lSDL2 -lSDL2_image -lassimp -lm
SRCS = main.c $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
MAIN = game.out

all: main

debug: CFLAGS += -g
debug: all

vpath %.c src
vpath %.h include

main: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(MAIN) $(OBJS)

%.o: main.c src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(MAIN) *.o */*.o
