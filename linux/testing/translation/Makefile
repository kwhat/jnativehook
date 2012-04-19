CC := gcc
CFLAGS := -O0 -ggdb -pipe -Wall -std=c99
LDFLAGS := -Wl,--as-needed,-O0

PROGRAMS = XkbTranslate X11Translate

all: $(PROGRAMS)

XkbTranslate: XkbTranslate.c
	$(CC) $(CFLAGS) -std=c99 -lX11 -lXtst $(LDFLAGS) $< -o $@

X11Translate: X11Translate.c
	$(CC) $(CFLAGS) -std=c99 -lX11 $(LDFLAGS) $< -o $@

clean:
	-rm -f $(PROGRAMS)
