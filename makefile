# Compiles main.c

.SUFFIXES: .c .o

CC = gcc

PKG_DEPS = glew sdl2 SDL2_image assimp
PKG_CFLAGS := $(shell pkg-config --cflags $(PKG_DEPS))
PKG_LIBS := $(shell pkg-config --libs $(PKG_DEPS))

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	GL_LIBS = -framework OpenGL
else
	GL_LIBS = -lGL
endif

CFLAGS = -Wall -Iinclude $(PKG_CFLAGS)
LIBS = $(PKG_LIBS) $(GL_LIBS) -lm
SRCS = main.c $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
MAIN = game.out

all: main

debug: CFLAGS += -g
debug: all

asan: CFLAGS += -g -O1 -fno-omit-frame-pointer -fsanitize=address,undefined
asan: LIBS += -fsanitize=address,undefined
asan: all

vpath %.c src
vpath %.h include

main: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(MAIN) $(OBJS)

%.o: main.c src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(MAIN) *.o */*.o
