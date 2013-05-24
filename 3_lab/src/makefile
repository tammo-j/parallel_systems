
CC=gcc
CFLAGS=-Wall
LIBS=-lOpenCL -lrt
BIN_DIR=../bin
BIN=cl_scan

compile: $(BIN_DIR)/$(BIN)

run: compile
	$(BIN_DIR)/$(BIN)

clean:
	rm -r $(BIN_DIR)

$(BIN_DIR)/$(BIN) : $(BIN_DIR)
	$(CC) $(CFLAGS) $(LIBS) -o $(BIN_DIR)/$(BIN) \
		cl_scan.c

$(BIN_DIR):
	mkdir $(BIN_DIR)
