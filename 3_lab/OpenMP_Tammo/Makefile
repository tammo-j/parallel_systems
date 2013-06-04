CC      = gcc
CFLAGS  = -std=c99 -Wall -g
LIBS    = -lm
OPTIONS = -fopenmp
SOURCES = test_mp.c
BIN_DIR = .
BIN     = test_mp

compile: $(BIN_DIR)/$(BIN)

.DEFAULT:
run: compile
	$(BIN_DIR)/$(BIN)

.PHONY:
clean:
	rm -r $(BIN_DIR)

debug: compile
	gdb $(BIN_DIR)/$(BIN)

$(BIN_DIR)/$(BIN) : $(BIN_DIR) $(SOURCES)
	$(CC) $(CFLAGS) $(OPTIONS) $(LIBS) -o $(BIN_DIR)/$(BIN) $(SOURCES)

$(BIN_DIR):
	mkdir $(BIN_DIR)
