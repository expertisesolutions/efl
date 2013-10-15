#! /bin/bash

S="../src/lib/eo/*.c"
I="-I../src/lib/eo -I../build -I../ -Ibuild -ggdb3 -DEFL_BETA_API_SUPPORT"
D="-DHAVE_CONFIG_H=1"
PKG_CONFIG_PATH=../../efl-upstream-install/lib/pkgconfig
L=`PKG_CONFIG_PATH=${PKG_CONFIG_PATH} pkg-config --cflags --libs eina`

CC=${CC:-gcc}

echo $L
$CC $I $CFLAGS eo_simple.c eo_inherit.c eo_composite.c eo2_simple.c eo2_inherit.c eo2_composite.c eo2-bench.c -DNOMAIN $S $I $D $L -std=gnu99 -lrt -o eo2-bench && ./eo2-bench || exit 1

#g++ $CFLAGS --std=c++0x simplesignal.cc `pkg-config --cflags --libs eina eo` -o simplesignal && ./simplesignal || exit 1
