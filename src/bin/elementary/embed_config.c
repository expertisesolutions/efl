#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_CONFIG1
# include "elementary_config.h"
#endif

static void
_print_usage(const char *progn)
{
   printf("Usage: %s [options] [input]\n", progn);
   printf("Options:\n"
                 "  -o output     specify the filename and path for output\n"
                 "  -v            show version number\n"
                 "\n");
}

char *
_read_input_file(FILE* f, const char* fname)
{
   fseek(f, 0, SEEK_END);
   long fs = ftell(f);
   if (fs < 0)
     {
        fprintf(stderr, "eolian: could not get length of '%s'\n", fname);
        return NULL;
     }
   fseek(f, 0, SEEK_SET);

   char *cont = malloc(fs + 1);
   if (!cont)
     {
        fprintf(stderr, "eolian: could not allocate memory for '%s'\n", fname);
        free(cont);
        return NULL;
     }

   long as = fread(cont, 1, fs, f);
   if (as != fs)
     {
        fprintf(stderr, "eolian: could not read %ld bytes from '%s' (got %ld)\n",
                fs, fname, as);
        free(cont);
        return NULL;
     }

   cont[fs] = '\0';
   return cont;
}

int
main(int argc, char** argv)
{
   char *output = NULL, *input = NULL;
   FILE *fin = NULL, *fout = NULL;
   int err = -1;
  
   for (int opt; (opt = getopt(argc, argv, "ho:")) != -1;)
     switch (opt)
       {
        case 0:
          break;
        case 'o':
          output = strdup(optarg);
          break;
        case 'h':
          _print_usage(argv[0]);
          return 0;
       }

   if (!argv[optind])
     {
        fprintf(stderr, "embed_config: no input file\n");
        goto clean_return;
     }
   else
     input = strdup(argv[optind]);

   fin = fopen(input, "rb");
   if (!fin)
     {
        fprintf(stderr, "embed_config: could not open '%s' (%s)\n",
                input, strerror(errno));
        goto clean_return;
     }

   fout = fopen(output, "wb");
   if (!fout)
     {
        fprintf(stderr, "embed_config: could not open '%s' (%s)\n",
                output, strerror(errno));
        goto clean_return;
     }

   char *input_buffer = _read_input_file(fin, input);
   if (input_buffer)
     {
        const char file_prefix[] = "static const char *embedded_config = \"\"\n";
        const char file_suffix[] = ";\n";
        fwrite(&file_prefix[0], sizeof(file_prefix) -1, 1, fout);
        unsigned int i = 0, eol = 0, eof = strlen(input_buffer);
        
        while (i != eof)
          {
            eol = i;
            while (eol != eof && (input_buffer[eol] != '\n' && input_buffer[eol] != '\r'))
              ++eol;
            assert (eol == eof || input_buffer[eol] == '\n' || input_buffer[eol] == '\r');
            fputc('"', fout);

            while (i != eol)
              {
                 if (input_buffer[i] == '"')
                   fputc('\\', fout);
                 fputc(input_buffer[i], fout);
                 ++i;
              }
            fputs("\\n\"", fout);
            if (i != eof)
              {
                 assert(i == eol);
                 assert(input_buffer[i] == '\n' || input_buffer[i] == '\r');
                 while (input_buffer[i] == '\r' || input_buffer[i] == '\n')
                   fputc(input_buffer[i++], fout);
              }
          }

        fwrite(&file_suffix[0], sizeof(file_suffix) -1, 1, fout);

        err = 0;
     }
   else
     fprintf(stderr, "embed_config: could not read from '%s'\n", input);

   free(input_buffer);

clean_return:
   if (fin != NULL) fclose(fin);
   if (fout != NULL) fclose(fout);
   free(input);
   free(output);
   return err;
}
