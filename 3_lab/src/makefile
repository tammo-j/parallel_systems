
CC      = gcc
CFLAGS  = -std=c99 -Wall -g
LIBS    = -lOpenCL -lrt
SOURCES = cl_scan.c
BIN_DIR = ../bin
BIN     = cl_scan

compile: $(BIN_DIR)/$(BIN)

run: compile
	$(BIN_DIR)/$(BIN)

clean:
	rm -r $(BIN_DIR)

debug: compile
	gdb $(BIN_DIR)/$(BIN)

$(BIN_DIR)/$(BIN) : $(BIN_DIR) $(SOURCES)
	$(CC) $(CFLAGS) $(LIBS) -o $(BIN_DIR)/$(BIN) $(SOURCES)

$(BIN_DIR):
	mkdir $(BIN_DIR)
