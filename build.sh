#!/bin/sh

gcc -c -o npoc_main.o npoc_main.c \
    $(pkg-config --cflags cairo)

gcc -c -o cairo_draw.o cairo_draw.c \
    $(pkg-config --cflags cairo)

gcc -c -o backend_x11.o backend_x11.c \
    $(pkg-config --cflags cairo) \
    $(pkg-config --cflags x11) 

gcc -o npoc_meter npoc_main.o cairo_draw.o backend_x11.o \
    $(pkg-config --libs cairo) \
    $(pkg-config --libs x11) 
