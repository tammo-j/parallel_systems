
CC=gcc
CFLAGS=-std=c99
BIN_DIR=../bin

serial_a:
	$(CC) $(CFLAGS) -o $(BIN_DIR)/conway_serial_a \
		conway_serial_a.c conway_algorithm_common.c \
		conway_algorithm_serial_a.c map_common.c map_array.c

serial_b:
	$(CC) $(CFLAGS) -o $(BIN_DIR)/conway_serial_b \
		conway_serial_b.c conway_algorithm_common.c \
		conway_algorithm_serial_b.c map_list.c

partial_a:
	$(CC) $(CFLAGS) -o $(BIN_DIR)/conway_partial_a \
		conway_partial_a.c conway_algorithm_common.c \
		conway_algorithm_partial_a.c map_common.c map_array.c

