//Compile with:
// gcc -o eet-data-cipher_decipher eet-data-cipher_decipher.c `pkg-config --cflags --libs eet eina`

#include <Eina.h>
#include <Eet.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#ifndef _MSC_VER
# include <sys/stat.h>
# include <unistd.h>
#endif
#include <string.h>

int
main(void)
{
   const char *buffer = "Here is a string of data to save !";
   const char *key = "This is a crypto key";
   const char *key_bad = "This is another crypto key";

   Eet_File *ef;
   char *test;
   int size;
   Eina_Tmpstr *tmpfile = NULL;
   int tmpfd;

   eet_init();

   /* tmpfile will be created in temporary directory (with eina_environment_tmp) */
   tmpfd = eina_file_mkstemp("eet_cipher_example_XXXXXX", &tmpfile);
   if ((-1 == tmpfd) || !!close(tmpfd))
     {
        fprintf(
                stderr, "ERROR: could not create temporary tmpfile (%s) : %s\n",
                tmpfile, strerror(errno));
        goto panic;
     }

   /* Crypt an eet file. */
   ef = eet_open(tmpfile, EET_FILE_MODE_WRITE);
   if (!ef)
     {
        fprintf(
          stderr, "ERROR: could not access tmpfile (%s).\n", tmpfile);
        goto error;
     }

   if (!eet_write_cipher(ef, "keys/tests", buffer, strlen(buffer) + 1, 0, key))
     {
        fprintf(
          stderr, "ERROR: could not access tmpfile (%s).\n", tmpfile);
        goto error;
     }

   eet_close(ef);

   /* Decrypt an eet file. */
   ef = eet_open(tmpfile, EET_FILE_MODE_READ);
   if (!ef)
     {
        fprintf(
          stderr, "ERROR: could not access tmpfile (%s).\n", tmpfile);
        goto error;
     }

   test = eet_read_cipher(ef, "keys/tests", &size, key);
   if (!test)
     {
        fprintf(
          stderr, "ERROR: could decript contents on file %s, with key %s.\n",
          tmpfile, key);
        goto error;
     }

   if (size != (int)strlen(buffer) + 1)
     {
        fprintf(
          stderr, "ERROR: something is wrong with the decripted data\n");
        goto error;
     }

   if (memcmp(test, buffer, strlen(buffer) + 1) != 0)
     {
        fprintf(
          stderr, "ERROR: something is wrong with the decripted data\n");
        goto error;
     }

   eet_close(ef);

   /* Decrypt an eet file, now using our BAD key!! */
   ef = eet_open(tmpfile, EET_FILE_MODE_READ);
   if (!ef)
     {
        fprintf(
          stderr, "ERROR: could not access tmpfile (%s).\n", tmpfile);
        goto error;
     }

   test = eet_read_cipher(ef, "keys/tests", &size, key_bad);

   if (size == (int)strlen(buffer) + 1)
     if (memcmp(test, buffer, strlen(buffer) + 1) == 0)
       {
          fprintf(
            stderr, "ERROR: something is wrong with the contents of %s, as"
                    " we accessed it with a different key and it decripted our"
                    " information right.\n", tmpfile);
          goto error;
       }

   eet_close(ef);

error:
   if (eina_file_unlink(tmpfile) != EINA_TRUE)
     {
        fprintf(
          stderr, "ERROR: could not unlink tmpfile (%s).\n", tmpfile);
     }

panic:
   eina_tmpstr_del(tmpfile);
   eet_shutdown();
}
