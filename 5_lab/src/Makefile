PROGS = main
CFLAGS += -Wall -std=c99 -g
LDLIBS += -fopenmp

EXTRASRC = fifo_b.c
EXTRADEP = fifo_b.h Makefile

.PHONY: all
all: $(PROGS)

.PHONY: clean
clean:
	rm -rf $(PROGS)

# Slightly modified defaults from GNU make 3.81
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH)
$(PROGS): % : %.c $(EXTRASRC) $(EXTRADEP)
	$(LINK.c) $< $(EXTRASRC) $(LOADLIBES) $(LDLIBS) -o $@

