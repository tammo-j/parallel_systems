PROGS = generate_random_matrix multiply_matrix show_matrix compare_matrix
CFLAGS += -Wall

EXTRASRC = matrix.c
EXTRADEP = matrix.h Makefile

.PHONY: all
all: $(PROGS)

.PHONY: clean
clean:
	rm -f $(PROGS)

# Slightly modified defaults from GNU make 3.81
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH)
$(PROGS): % : %.c $(EXTRASRC) $(EXTRADEP)
	$(LINK.c) $< $(EXTRASRC) $(LOADLIBES) $(LDLIBS) -o $@

