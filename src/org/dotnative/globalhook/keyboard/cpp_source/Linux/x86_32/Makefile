CC=g++
CFLAGs=-O2 -march=i586 -shared -fPIC

all: GlobalKeyHook

GlobalKeyHook: org_dotnative_globalhook_keyboard_GlobalKeyHook.cpp
	$(CC) -lX11 -lXevie -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux -o libGlobalKeyListener.so