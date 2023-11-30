CC = clang
LIB = lib/libraylib.a
OSLIB = -framework IOKit -framework Cocoa -framework OpenGL -framework CoreAudio
INCLUDE = include/

all: main.c render.o audio.o game.o
	make game
	make audio
	make render
	make main.o
	$(CC) main.o render.o audio.o game.o $(LIB) $(OSLIB)
.PHONY: all

main.o: main.c
	$(CC) -I $(INCLUDE) -c main.c -o main.o

game: game.c
	make game.o

game.o: game.c
	$(CC) -I $(INCLUDE) -c game.c -o game.o

game.h: game.c
	./makeheaders game.c -o=$(INCLUDE)
.PHONY: game.h

audio: audio.c
	make audio.h
	make audio.o
.PHONY: audio

audio.o: audio.c
	$(CC) -I $(INCLUDE) -c audio.c -o audio.o

audio.h: audio.c
	./makeheaders audio.c -o=$(INCLUDE)

render: render.c
	make render.o

render.o: render.c
	$(CC) -I $(INCLUDE) -c render.c -o render.o

render.h: render.c
	./makeheaders render.c -o=$(INCLUDE)

