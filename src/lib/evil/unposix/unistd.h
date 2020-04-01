#ifndef UNISTD_H
#define UNISTD_H

//#include "unimplemented.h"

#include <evil_unistd.h>

#define F_OK 0
#define X_OK 0
#define W_OK 2
#define R_OK 4

int getopt(int argc, char * const argv[], const char *optstring);

char *optarg = 0;
int optind  = 1, opterr  = 0, optopt  = 0;



//UNIMPLEMENTED extern char *optarg;
//UNIMPLEMENTED extern int optind, opterr, optopt;

#endif
