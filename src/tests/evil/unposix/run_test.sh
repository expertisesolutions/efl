#!/bin/sh


# clean and update
#\rm -f test_evil_unposix_strings.exe test_evil_unposix_strings.c
#\cp ../../../lib/evil/unposix/test_evil_unposix_strings.c . &&

# build
STRINGS_H_DIR="C:\Users\Carlos\source\repos\efl2\src\lib\evil\unposix"

PATH=$PATH:$PCRE_BIN_DIR
clang-cl -o test_evil_unposix_strings.exe         \
    test_evil_unposix_strings.c        \
    -I $STRINGS_H_DIR       

# test
chmod +x test_evil_unposix_strings.exe &&
./test_evil_unposix_strings.exe 

