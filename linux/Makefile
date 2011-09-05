CC := gcc
CFLAGS := -march=native -O0 -g2 -ggdb -pipe -Wall
LDFLAGS := -Wl,--as-needed,-O1

PROGRAMS = XEvIE XGrabButton XGrabKey XGrabPointer XRecord

all: $(PROGRAMS)

XEvIE: XEvIE.c
	$(CC) $(CFLAGS) -lX11 -lXevie $< -o $@

XGrabButton: XGrabButton.c
	$(CC) $(CFLAGS) -lXtst $< -o $@

XGrabKey: XGrabKey.c
	$(CC) $(CFLAGS) -lXtst $< -o $@
	
XGrabPointer: XGrabPointer.c
	$(CC) $(CFLAGS) -lXtst $< -o $@
	
XRecord: XRecord.c
	$(CC) $(CFLAGS) -lXtst $< -o $@

clean:
	-rm -f $(PROGRAMS)
