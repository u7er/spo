#!/bin/bash

gcc main.c -o main
gcc -shared -fPIC -o mtrace.so mtrace.c -ldl
LD_PRELOAD=./mtrace.so ./main
