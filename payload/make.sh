#!/bin/bash
gcc -Wall -dynamiclib -m32 -arch i386 -O2 -o payload.dylib main.c
