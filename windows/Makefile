CC := gcc
CFLAGS := -march=native -O0 -g2 -ggdb -pipe -Wall
LDFLAGS := -Wl,--as-needed,-O1

PROGRAMS = SetWindowsHookEx

all: $(PROGRAMS)

SetWindowsHookEx: SetWindowsHookEx.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	-del /F /Q $(PROGRAMS)
