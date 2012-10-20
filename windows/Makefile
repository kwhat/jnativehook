CC := gcc
CFLAGS := -O0 -g2 -ggdb -pipe -Wall
LDFLAGS := -Wl,--as-needed,-O0

PROGRAMS = RegisterHotKey.exe SetWindowsHookEx.exe PostMessage.exe

all: $(PROGRAMS)

RegisterHotKey.exe: RegisterHotKey.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

SetWindowsHookEx.exe: SetWindowsHookEx.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

PostMessage.exe  PostMessage.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	-del /F /Q $(PROGRAMS)
