CC := gcc
CFLAGS := -O0 -g2 -ggdb -pipe -Wall
LDFLAGS := -Wl,--as-needed,-O1

PROGRAMS = RegisterHotKey SetWindowsHookEx

all: $(PROGRAMS)

RegisterHotKey: RegisterHotKey.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

SetWindowsHookEx: SetWindowsHookEx.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	-del /F /Q $(PROGRAMS)
