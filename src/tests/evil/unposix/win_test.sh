#!/bin/sh

# clean and update
\rm -f vasprintf_test stdio.[hc]
\cp ../../../lib/evil/unposix/stdio.[hc] . &&

# build    
clang-cl vasprintf_test.c stdio.c -Wno-include-next-absolute-path -o vasprintf_test &&

# test
./vasprintf_test

# clean again
\rm -f vasprintf_test stdio.[hc]
