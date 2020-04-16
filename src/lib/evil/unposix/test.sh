#\rm -f fnmatch_test && gcc fnmatch_test.c fnmatch.c -lpcreposix -lpcre -o fnmatch_test && ./fnmatch_test
PCRE_SRC_DIR="/c/Users/joaoantoniocardo/Desktop/pcre-win-build"
PCRE_BIN_DIR=$PCRE_SRC_DIR/build-VS2019/x64/Release
PATH=$PATH:$PCRE_BIN_DIR
\rm -f fnmatch_test.exe && clang-cl -w -o fnmatch_test.exe fnmatch_test.c fnmatch.c $PCRE_BIN_DIR/libpcre.lib $PCRE_BIN_DIR/libpcreposix.lib -I $PCRE_BIN_DIR -I $PCRE_SRC_DIR && chmod +x fnmatch_ test.exe && ./fnmatch_test.exe
