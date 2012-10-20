CC := gcc
CFLAGS := -O0 -ggdb -pipe -Wall
LDFLAGS := -Wl,-execute

PROGRAMS = CreateKeyboardEvent EventMonitor EventTap RegisterEventHotKey

all: $(PROGRAMS)

CreateKeyboardEvent: CreateKeyboardEvent.c
	$(CC) $(CFLAGS) -framework ApplicationServices $(LDFLAGS) $< -o $@

EventMonitor: EventMonitor.c
	$(CC) $(CFLAGS) -framework Carbon $(LDFLAGS) $< -o $@

EventTap: EventTap.c
	$(CC) $(CFLAGS) -framework ApplicationServices $(LDFLAGS) $< -o $@

RegisterEventHotKey: RegisterEventHotKey.c
	$(CC) $(CFLAGS) -framework Carbon $(LDFLAGS) $< -o $@

clean:
	-rm -Rf $(PROGRAMS) *.dSYM
