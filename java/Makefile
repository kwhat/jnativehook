CC := javac
CFLAGS := -g -verbose -deprecation -source 1.5 -target 1.5

PROGRAMS = DesktopPropties

all: $(PROGRAMS)

DesktopPropties: DesktopPropties.java
	$(CC) $(CFLAGS) $<

clean:
	-rm -f *.class
