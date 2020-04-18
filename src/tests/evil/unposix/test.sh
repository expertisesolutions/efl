#!/bin/sh

# clean and update
\rm -f vasprintf_test_gnu vasprintf_test stdio.[hc]
\cp ../../../lib/evil/unposix/stdio.[hc] . &&

# build    
gcc vasprintf_test.c -DGNU -o vasprintf_test_gnu && 
gcc vasprintf_test.c stdio.c -DLINUX -I . -o vasprintf_test &&

# test
valgrind --leak-check=full ./vasprintf_test_gnu &&
valgrind --leak-check=full ./vasprintf_test

# clean again
\rm -f vasprintf_test_gnu vasprintf_test stdio.[hc]
