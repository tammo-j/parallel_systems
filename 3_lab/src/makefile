
CC      = gcc
CFLAGS  = -std=c99 -Wall
LIBS    = -lOpenCL -lrt
BIN_DIR = ../bin
BIN     = cl_scan
SOURCES = cl_scan.c

compile: $(BIN_DIR)/$(BIN)

run: compile
	$(BIN_DIR)/$(BIN)

clean:
	rm -r $(BIN_DIR)

$(BIN_DIR)/$(BIN) : $(BIN_DIR) $(SOURCES)
	$(CC) $(CFLAGS) $(LIBS) -o $(BIN_DIR)/$(BIN) $(SOURCES)

$(BIN_DIR):
	mkdir $(BIN_DIR)
