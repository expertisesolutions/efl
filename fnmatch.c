#include "fnmatch.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char * __wildcards_to_regex(const char *pattern, int flags);

inline char * __wildcards_to_regex(const char *pattern, int flags)
{

  printf("\tflags  : ");
  if (flags & FNM_NOESCAPE)
    printf("FNM_NOESCAPE,");
  if (flags & FNM_PERIOD)
    printf("FNM_PERIOD,");
  if (flags & FNM_PATHNAME)
    printf("FNM_PATHNAME,");
  if (flags & FNM_CASEFOLD)
    printf("FNM_CASEFOLD,");
  printf("\n");

  printf("\tpattern: %s\n", pattern);

  // counts how many additional chars to be allocated
  int count = 2; // starts with '^' and ends with '$'
  for (int i = 0; pattern[i] != '\0'; ++i) 
    {
      if (pattern[i] == '*')
        {
          if (flags & FNM_PATHNAME) count += 7;
          else count++;
          if (flags & FNM_PERIOD)
            {
              if (!(flags & FNM_NOESCAPE)) count++;
              count++;
            }
        }
      else if(pattern[i] == '?')
        {
          if (flags & FNM_PATHNAME) count += 3;
          else count++;
          if (flags & FNM_PERIOD)
            {
              if (!(flags & FNM_NOESCAPE)) count++;
              count++;
            }
        }
      else if(pattern[i] == '.')
      {
        count += 4;
      }
    }
  int pattern_length = strlen(pattern);
  int reg_length = pattern_length + count;

  // Translates wildcards to regex
  char *reg_pattern = (char*) malloc(reg_length * sizeof(char));
  reg_pattern[0] = '^';
  int i = 1;
    for (int j = 0; j < pattern_length; ++j) {

        if ( (pattern[j] == '*') || (pattern[j] == '?') ) { // '*' and '?'
            if (flags & FNM_PATHNAME) {
                if (flags & FNM_PERIOD) {                   // PATHNAME + PERIOD
                    if (pattern[j] == '*') {
                        if (j == 0) {
                            strcpy(reg_pattern+i, "[^\\.][^/]*"); i+=10;
                        } else {
                            if (pattern[j-1] == '/') {
                                strcpy(reg_pattern+i, "[^\\.][^/]*"); i+=10;
                            } else {
                                strcpy(reg_pattern+i, "[^/]*"); i+=5;
                            }
                        }
                    } else {
                        if (j == 0) {
                            strcpy(reg_pattern+i, "[^\\.][^/]?"); i+=10;
                        } else {
                            if (pattern[j-1] == '/') {
                                strcpy(reg_pattern+i, "[^\\.][^/]?"); i+=10;
                            } else {
                                strcpy(reg_pattern+i, "[^/]"); i+=4;
                            }
                        }
                    }
                } else {                                    // PATHNAME
                    if (pattern[j] == '*') {
                        strcpy(reg_pattern+i, "[^/]*"); i+=5;
                    } else {
                        strcpy(reg_pattern+i, "[^/]"); i+=4;
                    }
                }
            } else if (flags & FNM_PERIOD) {                // PERIOD
                if (j == 0) {                                   // at init
                    if (pattern[j] == '*') {                    
                        strcpy(reg_pattern+i, "[\\.]*"); i+=5;
                    } else {
                        strcpy(reg_pattern+i, "[\\.]"); i+=4;
                    }
                } else {                                        // other places
                    if (pattern[j] == '*') {
                        strcpy(reg_pattern+i, ".*"); i+=2;
                    } else {
                        strcpy(reg_pattern+i, "."); i+=1;
                    }
                }
            } else {                                        // NORMAL
                if (pattern[j] == '*') {
                    strcpy(reg_pattern+i, ".*"); i+=2;
                } else {
                    strcpy(reg_pattern+i, "."); i+=1;
                }
            }
        } else if (pattern[j] == '.') {                 // '.'
            if (flags & FNM_PERIOD) {                       // PERIOD
                strcpy(reg_pattern+i, "[\\.]"); i+=4;
            }
        } else {                                        // OTHERS
            if (pattern[j] == '\\') {
                reg_pattern[i++] = '/';
            } else {
                reg_pattern[i++] = pattern[j];
            }
            
        }

    }
  strcpy(reg_pattern + i, "$\0");

  return reg_pattern;
}

int fnmatch (const char *pattern, const char *string, int flags)
{
  regex_t regex;          
  int result;

  // Converts wildcard pattern to regex pattern
  char *reg_pattern = __wildcards_to_regex(pattern, flags);
 
  // Configures regex
  int regex_flags = (REG_NOSUB) // Report only success/fail in regexec()
    || ((flags & FNM_CASEFOLD)? REG_ICASE : 0) ;

  // Compiles regex
  if (regcomp(&regex, reg_pattern, regex_flags)) {
    regfree(&regex);
    return FNM_NOMATCH;
  }

  // Replaces '\\' with '/'
  char *unix_path = (char*) malloc(strlen(string) * sizeof(char));
  for (int i = 0; string[i] != '\0'; ++i)
    {
      unix_path[i] = (string[i] == '\\')? '/' : string[i];
    }

  // Executes regex
  printf("\tregex  : %s\n", reg_pattern);
  printf("\tstring : %s\n", string);
  printf("\tstring : %s\n", unix_path);
  result = regexec(&regex, unix_path, 0, NULL, 0);

  // Cleans-up and returns
  //free(unix_path);
  free(reg_pattern);
  regfree(&regex);
  return result? FNM_NOMATCH : 0;
}

