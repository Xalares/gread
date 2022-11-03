#!/bin/bash
valgrind --leak-check=full --leak-resolution=high --num-callers=20 --log-file=vgdump.txt --suppressions=/usr/share/glib-2.0/valgrind/glib.supp --suppressions=/usr/share/gtk-4.0/valgrind/gtk.supp ./build/src/gread
