#\rm -f fnmatch_test && gcc -D ORIGINAL_FNMATCH fnmatch_test.c -o fnmatch_test && ./fnmatch_test
\rm -f fnmatch_test && gcc fnmatch_test.c fnmatch.c -lpcreposix -lpcre -o fnmatch_test && ./fnmatch_test
