#!/bin/sh

# clean and update
\rm -f vasprintf_test_gnu vasprintf_test stdio.[hc]
\cp ../../../lib/evil/unposix/stdio.[hc] . &&

if [[ "$OSTYPE" == "linux-gnu"  ]]; then
    # build    
    gcc vasprintf_test.c -DGNU -o vasprintf_test_gnu && 
    gcc vasprintf_test.c stdio.c -DLINUX -I . -o vasprintf_test &&

    # test
    valgrind --leak-check=full ./vasprintf_test_gnu &&
    valgrind --leak-check=full ./vasprintf_test

elif [[ "$OSTYPE" == "msys"  ]]; then
    # build
    clang-cl vasprintf_test.c stdio.c -Wno-include-next-absolute-path -o vasprintf_test &&

    # test
    ./vasprintf_test
fi

# clean again
\rm -f vasprintf_test_gnu vasprintf_test stdio.[hc]
