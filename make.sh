#!/bin/sh

gcc -o my_test cairo-example.c \
    $(pkg-config --cflags --libs cairo) \
    $(pkg-config --cflags --libs x11) 
