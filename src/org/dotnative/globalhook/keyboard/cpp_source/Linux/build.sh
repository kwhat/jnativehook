#!/bin/bash

g++ -m32 -march=i586 -shared -fPIC -lX11 -lXevie -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux ./org_dotnative_globalhook_keyboard_GlobalKeyHook.cpp -o ./x86_32/libGlobalKeyListener.so
g++ -m64 -march=k8 -shared -fPIC -lX11 -lXevie -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux ./org_dotnative_globalhook_keyboard_GlobalKeyHook.cpp -o ./x86_64/libGlobalKeyListener.so
