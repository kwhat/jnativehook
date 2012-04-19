CC := gcc
CFLAGS := -O0 -ggdb -pipe -Wall -std=c99
LDFLAGS := -Wl,--as-needed,-O0

PROGRAMS = XGetModifierMapping XGetKeyboardMapping XGetPointerMapping XkbKeySymEntry

all: $(PROGRAMS)

XGetModifierMapping: XGetModifierMapping.c
	$(CC) $(CFLAGS) -lX11 $(LDFLAGS) $< -o $@

XGetKeyboardMapping: XGetKeyboardMapping.c
	$(CC) $(CFLAGS) -lX11 $(LDFLAGS) $< -o $@

XGetPointerMapping: XGetPointerMapping.c
	$(CC) $(CFLAGS) -lX11 $(LDFLAGS) $< -o $@

XkbKeySymEntry: XkbKeySymEntry.c
	$(CC) $(CFLAGS) -lX11 -lXtst $(LDFLAGS) $< -o $@

clean:
	-rm -f $(PROGRAMS)
