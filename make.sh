#!/bin/sh

gcc -c -o cairo-example.o cairo-example.c \
    $(pkg-config --cflags cairo) \
    $(pkg-config --cflags x11) 

gcc -c -o cairo_draw.o cairo_draw.c \
    $(pkg-config --cflags cairo) \
    $(pkg-config --cflags x11) 

gcc -o my_test cairo-example.o cairo_draw.o\
    $(pkg-config --libs cairo) \
    $(pkg-config --libs x11) 
