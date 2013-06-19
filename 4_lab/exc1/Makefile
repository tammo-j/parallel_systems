PROGS = generate_random_matrix multiply_matrix show_matrix compare_matrix benchmark
CFLAGS += -Wall -std=c99 -O3 -funroll-loops -march=native
LDLIBS += -lm -fopenmp
OUT_DIR = ./v$(VER)

EXTRASRC = stopwatch.c matrix.c matrix_v$(VER).c
EXTRADEP = matrix.h Makefile

.PHONY: all
all: $(PROGS)

.PHONY: clean
clean:
	rm -rf $(OUT_DIR)[0123456789]

# Slightly modified defaults from GNU make 3.81
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH)
$(PROGS): % : %.c $(EXTRASRC) $(EXTRADEP) $(OUT_DIR)
	$(LINK.c) $< $(EXTRASRC) $(LOADLIBES) $(LDLIBS) -o $(OUT_DIR)/$@

$(OUT_DIR):
	mkdir $(OUT_DIR)
