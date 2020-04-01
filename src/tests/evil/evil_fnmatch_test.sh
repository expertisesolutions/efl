#!/bin/sh

if [[ "$OSTYPE" == "linux-gnu"  ]]; then

    # clean and update
    \rm -f fnmatch_test evil_fnmatch_test evil_fnmatch.[hc]
    \cp ../../lib/eina/evil_fnmatch.[hc] . &&

    # using strcpy instead of strcpy_s on linux
    sed -i '6i#define strcpy_s(a,b,c) strcpy(a,c)\n' evil_fnmatch.c

    # build
    gcc evil_fnmatch_test.c -o fnmatch_test
    gcc evil_fnmatch_test.c evil_fnmatch.c \
        -lpcreposix -lpcre       		\
        -o evil_fnmatch_test

    # test
    chmod +x fnmatch_test evil_fnmatch_test
    ./fnmatch_test
    ./evil_fnmatch_test

    # clean again
    \rm -f fnmatch_test evil_fnmatch_test evil_fnmatch.[hc]

elif [[ "$OSTYPE" == "msys"  ]]; then

    # clean and update
    \rm -f evil_fnmatch_test.exe evil_fnmatch.[hc]
    \cp ../../lib/eina/evil_fnmatch.[hc] . &&

    ### USING WRAP DEPENDENCY BUILD:
    # currently not working here :(
    # remember to change libpcre*.lib to libpcre*.a 
    # build dependency: pcre
    #PCRE_SRC_DIR=$(pwd)/"../../../subprojects/pcre-8.37"
    #PCRE_BIN_DIR=$PCRE_SRC_DIR/build
    #cd $PCRE_SRC_DIR && meson configure && meson --wipe build && cd build && ninja &&
    #cd ../../../src/tests/eina/

    ### OR USING CUSTOM BUILD:
    PCRE_SRC_DIR="/c/Users/joaoantoniocardo/Desktop/pcre-win-build"
    PCRE_BIN_DIR=$PCRE_SRC_DIR/build-VS2019/x64/Release

    # build
    PATH=$PATH:$PCRE_BIN_DIR
    clang-cl -o evil_fnmatch_test.exe         	\
        $PCRE_BIN_DIR/libpcre*.lib   	\
        evil_fnmatch_test.c evil_fnmatch.c     \
        -I $PCRE_SRC_DIR &&

    # test
    chmod +x evil_fnmatch_test.exe &&
    ./evil_fnmatch_test.exe

    # clean again
    \rm -rf evil_fnmatch_test.exe evil_fnmatch.[hc]

fi




