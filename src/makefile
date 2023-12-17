OS := $(shell uname)

ifeq ($(OS), Darwin)
    CC = clang
else
    CC = gcc
endif

LIB = lib/libraylib.a
OSLIB = -framework IOKit -framework Cocoa -framework OpenGL -framework CoreAudio
INCLUDE = include/
OPTIONS = 

all: main.o render.o audio.o game.o
	make main
.PHONY: all

main: main.o render.o audio.o game.o
	$(CC) main.o render.o audio.o game.o $(LIB) $(OSLIB) -o main $(OPTIONS)

main.o: main.c
	$(CC) -I $(INCLUDE) -c main.c -o main.o $(OPTIONS)

game: game.c
	make game.o

game.o: game.c
	$(CC) -I $(INCLUDE) -c game.c -o game.o $(OPTIONS)

game.h: game.c
	./makeheaders game.c -o=$(INCLUDE)

audio.o: audio.c
	$(CC) -I $(INCLUDE) -c audio.c -o audio.o $(OPTIONS)

audio.h: audio.c
	./makeheaders audio.c -o=$(INCLUDE) 

render.o: render.c
	$(CC) -I $(INCLUDE) -c render.c -o render.o $(OPTIONS)

render.h: render.c
	./makeheaders render.c -o=$(INCLUDE)

clean: 
	rm render.o 
	rm audio.o 
	rm game.o 
	rm main.o 
.PHONY: clean