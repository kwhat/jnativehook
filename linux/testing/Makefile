CC := gcc
CFLAGS := -O0 -g2 -ggdb -pipe -Wall
LDFLAGS := -Wl,--as-needed,-O1

PROGRAMS = XGetKeyboardMapping XGetPointerMapping

all: $(PROGRAMS)

XGetKeyboardMapping: XGetKeyboardMapping.c
	$(CC) $(CFLAGS) -lXtst $(LDFLAGS) $< -o $@

XGetPointerMapping: XGetPointerMapping.c
	$(CC) $(CFLAGS) -lXtst $(LDFLAGS) $< -o $@

clean:
	-rm -f $(PROGRAMS)
