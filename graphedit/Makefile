CC = g++

CFLAGS = -g -c 
LDFLAGS = -l raylib

SRC = $(wildcard src/*.cc)
HDR = $(wildcard src/*.h)
OBJ = $(SRC:src/%.cc=bin/obj/%.o)
BIN = bin/graphed

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

bin/obj/%.o: src/%.cc $(HDR)
	$(CC) -o $@ $< $(CFLAGS)

clean: 
	rm -rf bin/*
	mkdir bin/obj
