CC=gcc
CFLAGS=-Wall -O3 -fno-strict-aliasing -shared -fPIC
MARCH=
BITS=

include foo *.mk $(bar)

LIBS=
INCLUDES=

OSNAME = $(shell uname -s)
PLATFORM=linux
MARCH=$(shell uname -m)
LIBS=$(shell pkg-config x11 --libs) $(shell pkg-config xevie --libs)
INCLUDES=-I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux

ifeq ($(patsubst i%86,i586,$(MARCH)),i586)
	MARCH=i586
	BITS=32
else
	ifeq ($(MARCH),x86_64)
		MARCH=k8
		BITS=64
	else
		die "Cannot determine system architecture!"
	endif
endif

all: build

build:
	$(CC) $(CFLAGS) -m$(BITS) -march=$(MARCH) $(LIBS) $(INCLUDES) ./org_dotnative_globalhook_keyboard_GlobalKeyHook.cpp -o ./x86_$(BITS)/libGlobalHook_Keyboard.so
	
debug:
	$(CC) $(CFLAGS) -DDEBUG -m$(BITS) -march=$(MARCH) $(LIBS) $(INCLUDES) ./org_dotnative_globalhook_keyboard_GlobalKeyHook.cpp -o ./x86_$(BITS)/libGlobalHook_Keyboard.so

