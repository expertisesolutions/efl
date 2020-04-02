#!/bin/sh


\cp ../../../lib/evil/evil_strings.[hc] .

PATH=$PATH:$PCRE_BIN_DIR
clang-cl -o test_evil_unposix_strings.exe         \
    test_evil_unposix_strings.c        evil_strings.c\
    
    

# test
chmod +x test_evil_unposix_strings.exe &&
./test_evil_unposix_strings.exe &&
rm -f test_evil_unposix_strings.exe evil_string.[hc] evil_strings.[hc] evil_private.h
