#!/bin/bash

g++ -m32 -march=i586 -shared -fPIC -lX11 -lXevie -I/opt/sun-jdk-1.5.0.12/include -I/opt/sun-jdk-1.5.0.12/include/linux ./org_dotnative_globalhook_keyboard_GlobalKeyHook.cpp -o libGlobalKeyListener.so
