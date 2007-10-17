#!/bin/bash

g++ -m32 -o test.bin -march=i586 -lX11 -lXevie ./test.cpp

./test.bin &
PID=$!
sleep 10;
kill -9 $PID
