#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
#include <fcntl.h>

#ifndef _WIN32
#include <unistd.h>
#else
# include <evil_private.h>
#endif

#include <Eina.h>
#include <Eet.h>

#include "eet_suite.h"
#include "eet_test_common.h"

EFL_START_TEST(eet_test_cipher_decipher_simple)
{
   const char *buffer = "Here is a string of data to save !";
   const char *key = "This is a crypto key";
   const char *key_bad = "This is another crypto key";
   Eet_File *ef;
   char *test;
   int size;
   Eina_Tmpstr* tmpfile = NULL;
   int tmpfd;

   /* tmpfile will be created in temporary directory (with eina_environment_tmp) */
   tmpfd = eina_file_mkstemp("eet_suite_testXXXXXX", &tmpfile);
   fail_if(-1 == tmpfd);
   fail_if(!!close(tmpfd));

   /* Crypt an eet file. */
   ef = eet_open(tmpfile, EET_FILE_MODE_WRITE);
   fail_if(!ef);

   fail_if(!eet_write_cipher(ef, "keys/tests", buffer, strlen(buffer) + 1, 0,
                             key));

   eet_close(ef);

   /* Decrypt an eet file. */
   ef = eet_open(tmpfile, EET_FILE_MODE_READ);
   fail_if(!ef);

   test = eet_read_cipher(ef, "keys/tests", &size, key);
   fail_if(!test);
   fail_if(size != (int)strlen(buffer) + 1);

   fail_if(memcmp(test, buffer, strlen(buffer) + 1) != 0);

   eet_close(ef);

   /* Decrypt an eet file. */
   ef = eet_open(tmpfile, EET_FILE_MODE_READ);
   fail_if(!ef);

   test = eet_read_cipher(ef, "keys/tests", &size, key_bad);

   if (size == (int)strlen(buffer) + 1)
     fail_if(memcmp(test, buffer, strlen(buffer) + 1) == 0);

   eet_close(ef);
   /* As `eet_close` is a postponed close and windows' `unlink` doesn't execute
    * * successfully if there is any reference to the file, here `eet_clearcache`
    * is       * used to assure that the file is really closed when the unlink
    * happens.      */
   eet_clearcache();

   fail_if(unlink(tmpfile) != 0);
   eina_tmpstr_del(tmpfile);

}
EFL_END_TEST

void eet_test_cipher(TCase *tc)
{
   tcase_add_test(tc, eet_test_cipher_decipher_simple);
}
