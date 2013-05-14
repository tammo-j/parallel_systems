
CC=gcc
CFLAGS=-std=c99 -g -Wall
BIN_DIR=../bin
BIN=conway_b

compile:
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(BIN) \
		main.c map.c cell_list.c algorithm.c intermap.c communication.c

run: $(BIN_DIR)/$(BIN)
	$(BIN_DIR)/$(BIN)

$(BIN_DIR)/$(BIN): compile

