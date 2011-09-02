CC := gcc
CFLAGS := -march=native -O0 -g2 -ggdb -pipe
LDFLAGS := -Wl,--as-needed,-O1

PROGRAMS = EventMonitor

all: $(PROGRAMS)

EventMonitor: EventMonitor.c EventMonitor.h
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

clean:
	-rm -f $(PROGRAMS)
