CC := gcc
CFLAGS := -O0 -g2 -ggdb -pipe -Wall
LDFLAGS := -Wl,-execute

PROGRAMS = EventMonitor EventTap RegisterEventHotKey

all: $(PROGRAMS)

EventMonitor: EventMonitor.c
	$(CC) $(CFLAGS) -framework Carbon $(LDFLAGS) $< -o $@

EventTap: EventTap.c
	$(CC) $(CFLAGS) -framework Carbon $(LDFLAGS) $< -o $@

RegisterEventHotKey: RegisterEventHotKey.c
	$(CC) $(CFLAGS) -framework Carbon $(LDFLAGS) $< -o $@

clean:
	-rm -Rf $(PROGRAMS) *.dSYM
