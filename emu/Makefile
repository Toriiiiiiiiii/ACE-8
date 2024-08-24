CC = gcc

CCFLAGS = -g -c -Wall
LDFLAGS = -lraylib

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=bin/obj/%.o)
HDR = $(wildcard src/*h)

BIN = bin/emu

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

bin/obj/%.o: src/%.c $(HDR)
	$(CC) -o $@ $< $(CCFLAGS)
