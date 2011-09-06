CC := gcc
CFLAGS := -O0 -g2 -ggdb -pipe -Wall
LDFLAGS := -Wl,--as-needed,-O1

PROGRAMS = EventMonitor RegisterEventHotKey

all: $(PROGRAMS)

EventMonitor: EventMonitor.c EventMonitor.h
	$(CC) $(CFLAGS) -framework Carbon $< -o $@

RegisterEventHotKey: RegisterEventHotKey.c
	$(CC) $(CFLAGS) -framework Carbon $< -o $@

clean:
	-rm -f $(PROGRAMS)
