
CC=gcc
CFLAGS=-Wall
LIBS=-lOpenCL -lrt
BIN_DIR=../bin
BIN=template

compile: $(BIN_DIR)
	$(CC) $(CFLAGS) $(LIBS) -o $(BIN_DIR)/$(BIN) \
		template.c

run: $(BIN_DIR)/$(BIN)
	$(BIN_DIR)/$(BIN)

$(BIN_DIR)/$(BIN): $(BIN_DIR) compile

$(BIN_DIR):
	mkdir $(BIN_DIR)
