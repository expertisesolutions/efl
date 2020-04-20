#ifndef UNISTD_H
#define UNISTD_H

#include "unimplemented.h"
#include <evil_unistd.h>
#include <io.h>
#include <process.h>

#define F_OK 0
#define X_OK 0
#define W_OK 2
#define R_OK 4
#define STDIN_FILENO  0 
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
char optarg;
int optind; //temporary
int opterr; //temporary
int optopt; //temporary
#endif
