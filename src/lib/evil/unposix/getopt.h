#ifndef UNPOSIX_GETOPT_H
#define UNPOSIX_GETOPT_H

#include "unimplemented.h"

#include <unistd.h>

#include_next <getopt.h>

UNIMPLEMENTED int getopt_long(int argc, char * const argv[],
          const char *optstring,
          const struct option *longopts, int *longindex)
{
    #warning getopt_long is not implemented
    return 0;
}
