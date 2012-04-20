CC := gcc
CFLAGS := -O0 -ggdb -pipe -Wall
LDFLAGS := -Wl,-execute

PROGRAMS = TISTranslate CarbonTranslate

all: $(PROGRAMS)

TISTranslate: TISTranslate.c
	$(CC) $(CFLAGS) -framework ApplicationServices -framework Carbon $(LDFLAGS) $< -o $@

CarbonTranslate: CarbonTranslate.c
	$(CC) $(CFLAGS) -framework ApplicationServices -framework Carbon $(LDFLAGS) $< -o $@

clean:
	-rm -Rf $(PROGRAMS) *.dSYM
