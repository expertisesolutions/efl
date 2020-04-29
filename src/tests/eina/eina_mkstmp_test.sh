#!/bin/sh

# Clean
\rm -f eina_stdlib.[hc] mkstmp_test eina_mkstmp_test

# Prepare
\cp ../../lib/eina/eina_stdlib.[hc] . && 

# Build
gcc -Wall -o mkstmp_test eina_mkstmp_test.c &&
#gcc -Wall -o eina_mkstmp_test eina_mkstmp_test.c eina_mkstmp.c &&

# Run
./mkstmp_test
#./eina_mkstmp_test

# Clean again
\rm -f eina_stdlib.[hc] mkstmp_test eina_mkstmp_test
