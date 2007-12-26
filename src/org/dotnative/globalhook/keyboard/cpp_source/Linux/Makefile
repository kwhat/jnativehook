CC=g++
CFLAGS=-Wall -shared -fPIC
MARCH=
BITS=

LIBS=
INCLUDES=-I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux

OSNAME = $(shell uname -s)
ifeq ($(OSNAME), Linux)
	PLATFORM=linux
	MARCH=$(shell uname -m)
	LIBS=$(shell pkg-config x11 --libs) $(shell pkg-config xevie --libs)
	
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
endif


ifeq ($(OSNAME), SunOS)
	PLATFORM = solaris
	JCT_PLATFORM = solaris
	ARCH = $(shell uname -p)
	ifeq ($(ARCH), i386)
		ARCH=i586
	endif
endif

ifeq ($(OSNAME), Windows_NT)
	PLATFORM = windows
	JCT_PLATFORM = win32
	ifeq ($(word 1, $(PROCESSOR_IDENTIFIER)),ia64)
		ARCH=ia64
	else
		ifeq ($(word 1, $(PROCESSOR_IDENTIFIER)),AMD64)
			ARCH=x64
		else
			ifeq ($(word 1, $(PROCESSOR_IDENTIFIER)),EM64T)
				ARCH=x64
			else
				ARCH=i586
			endif
		endif
	endif
endif


all: clean build

build:
	$(CC) $(CFLAGS) -m$(BITS) -march=$(MARCH) $(LIBS) $(INCLUDES) ./org_dotnative_globalhook_keyboard_GlobalKeyHook.cpp -o ./x86_$(BITS)/libGlobalKeyListener.so
	
debug:
	$(CC) $(CFLAGS) -DDEBUG -m$(BITS) -march=$(MARCH) $(LIBS) $(INCLUDES) ./org_dotnative_globalhook_keyboard_GlobalKeyHook.cpp -o ./x86_$(BITS)/libGlobalKeyListener.so
	
clean:
	rm -rvf ./x86_$(BITS)/*.so
