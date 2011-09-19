CC := gcc
CFLAGS := -O0 -g2 -ggdb -pipe -Wall
LDFLAGS := -Wl,--as-needed,-O0

PROGRAMS = RegisterHotKey.exe SetWindowsHookEx.exe HookExCallback.dll

all: $(PROGRAMS)

RegisterHotKey.exe: RegisterHotKey.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

SetWindowsHookEx.exe: SetWindowsHookEx.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

HookExCallback.dll: HookExCallback.c
	$(CC) $(CFLAGS) -shared $(LDFLAGS) $< -o $@

clean:
	-del /F /Q $(PROGRAMS)
