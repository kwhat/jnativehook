CC := gcc
CFLAGS := -O0 -g2 -ggdb -pipe -Wall
LDFLAGS := -Wl,--as-needed,-O1

PROGRAMS = XEvIE XGrabButton XGrabKey XGrabPointer XRecord

all: $(PROGRAMS)

XEvIE: XEvIE.c
	$(CC) $(CFLAGS) $(LDFLAGS) -lX11 -lXevie $< -o $@

XGrabButton: XGrabButton.c
	$(CC) $(CFLAGS) -lXtst $(LDFLAGS) $< -o $@

XGrabKey: XGrabKey.c
	$(CC) $(CFLAGS) -lXtst $(LDFLAGS) $< -o $@
	
XGrabPointer: XGrabPointer.c
	$(CC) $(CFLAGS) -lXtst $(LDFLAGS) $< -o $@
	
XRecord: XRecord.c
	$(CC) $(CFLAGS) -lXtst $(LDFLAGS) $< -o $@

clean:
	-rm -f $(PROGRAMS)
