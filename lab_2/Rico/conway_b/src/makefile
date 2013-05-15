
CC=mpicc
CFLAGS=-std=c99 -g -Wall
BIN_DIR=../bin
BIN=conway_b
PROCESSES=6

compile:
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(BIN) \
		main.c grid.c map.c cell_list.c algorithm.c intermap.c communication.c

run-local: $(BIN_DIR)/$(BIN)
	mpirun -n $(PROCESSES) $(BIN_DIR)/$(BIN)
	
run-nmpi: $(BIN_DIR)/$(BIN)
	ccsalloc -n $(PROCESSES) nmpi -- $(BIN_DIR)/$(BIN)
	
debug: $(BIN_DIR)/$(BIN)
	mpirun -n $(PROCESSES) xterm -e gdb $(BIN_DIR)/$(BIN)

$(BIN_DIR)/$(BIN): $(BIN_DIR) compile

$(BIN_DIR):
	mkdir $(BIN_DIR)

