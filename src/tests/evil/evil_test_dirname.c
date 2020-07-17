#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <evil_private.h>

#include "evil_suite.h"

EFL_START_TEST(evil_dirname_simple)
{
   char path[] = "/bin/foo/bar.h";
   char expected[] = "/bin/foo";
   char *res = evil_dirname(path);

   fail_if(strcmp(res, expected) != 0);
}
EFL_END_TEST

EFL_START_TEST(evil_dirname_trailing)
{
   char path[] = "/bin/foo/";
   char expected[] = "/bin/foo";

   char *res = evil_dirname(path);

   fail_if(strcmp(res, expected) != 0);
}
EFL_END_TEST

EFL_START_TEST(evil_dirname_empty)
{
   char path[] = "";
   char expected[] = ".";
   char *res = evil_dirname(path);

   fail_if(strcmp(res, expected) != 0);
}
EFL_END_TEST

EFL_START_TEST(evil_dirname_null)
{
   char expected[] = ".";
   char *res = evil_dirname(NULL);

   fail_if(strcmp(res, expected) != 0);
}
EFL_END_TEST

EFL_START_TEST(evil_dirname_filename)
{
   char path[] = "bar.h";
   char expected[] = "";
   char *res = evil_dirname(path);

   fail_if(strcmp(res, expected) != 0);
}
EFL_END_TEST

void evil_test_dirname(TCase *tc)
{
   tcase_add_test(tc, evil_dirname_simple);
   tcase_add_test(tc, evil_dirname_trailing);
   tcase_add_test(tc, evil_dirname_empty);
   tcase_add_test(tc, evil_dirname_null);
   tcase_add_test(tc, evil_dirname_filename);
}
