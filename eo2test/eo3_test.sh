#!/bin/bash

export LD_LIBRARY_PATH=.

./simple1 >& /dev/null || echo "simple1 failed."
./simple2 >& /dev/null || echo "simple2 failed."
./simple3 >& /dev/null || echo "simple3 failed."
./simple4 >& /dev/null || echo "simple4 failed."
./simple5 >& /dev/null || echo "simple5 failed."
./simple6 >& /dev/null || echo "simple6 failed."
./mixin1 >& /dev/null || echo "mixin1 failed."
./mixin2 >& /dev/null || echo "mixin2 failed."
./container1 >& /dev/null || echo "container1 failed."


