#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <errno.h>
#include <unistd.h>

#include "eina_stdlib.h"


// ref: https://www.thegeekstuff.com/2012/06/c-temporary-files/


int
mkstemp_simple_test(void)
{
   printf("Testing mkstmp():\n");
   // Creates tempfile

   char temp_file_name[32];
   memset(temp_file_name, 0, sizeof(temp_file_name));

   const char template[] = "my_template-XXXXXX";
   strncpy(temp_file_name, template, strlen(template));

   int fd = mkstemp(temp_file_name);
   unlink(temp_file_name);

   if (fd == -1)
   {
      printf("\tCreation of temp file failed with error [%s]\n", 
            strerror(errno));
      return -1;
   }
   printf("\tTemporary file [%s] created\n", temp_file_name);


   // Writes to tempfile

   char write_buffer[24];
   memset(write_buffer, 0, sizeof(write_buffer));

   const char test_str[] = "Hello World!";
   int test_str_length = strlen(test_str);
   strncpy(write_buffer, test_str, test_str_length);

   int number_of_bytes_written = write(fd, write_buffer, 
                                       test_str_length);
   if (number_of_bytes_written != test_str_length)
   {
      printf("\tWrite failed with error [%s]\n", strerror(errno));
      return -1;
   }
   printf("\tData written to temporary file is [%s]\n", write_buffer);


   // Reads from tempfile
   
   char read_buffer[24];
   memset(read_buffer, 0, sizeof(read_buffer));

   if (lseek(fd, 0, SEEK_SET) == -1)
   {
      printf("\tlseek() failed with error [%s]\n", strerror(errno));
      return -1;
   }

   int number_of_bytes_read = read(fd, read_buffer, sizeof(read_buffer));
   if (number_of_bytes_read != test_str_length)
   {
      printf("\tRead failed with error [%s]\n", strerror(errno));
      return -1;
   }
   printf("\tData read back from temporary file is [%s]\n", read_buffer);


   // Assert
   if (strcmp(read_buffer, write_buffer) != 0)
   {
      printf("\tFailed: the data read differs from data written in tempfile.\n");
      return -1;
   }
   printf("\tSuccess: the data read is equals to the data written in tempfile.\n");

   printf("\tResult: Pass!\n");

   return 0;
}

int
main(void)
{

   mkstemp_simple_test();

   return 0;
}
