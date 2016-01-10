#!/bin/bash
gcc -m32 -arch i386 -Wall -O2 -o hax main.c common.c payload.c memory.c
./hax
