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
	
	ifeq ($(pathsubst %_64,,$(MARCH)),)
		MARCH=k8
		BITS=64
	else
		ifeq ($(pathsubst %86,,$(MARCH)),)
			MARCH=i586
			BITS=32
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


all:
	$(CC) $(CFLAGS) -m$(BITS) -march=$(MARCH) $(LIBS) $(INCLUDES) ./org_dotnative_globalhook_keyboard_GlobalKeyHook.cpp -o ./x86_$(BITS)/libGlobalKeyListener.so

clean:
	rm -rf *.so
