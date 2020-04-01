
#include "unistd.h"



int getopt(int argc, char * const argv[], const char *optstring){
    if (optind >= argc || !argv[optind]){
        optind = 1;
        return -1;
    }
	int caracter;
	if(argv[optind][0] == '-'){
	    caracter = argv[optind][1];
	    int optstring_len = sizeof(optstring);
	    for(int i = 0; i < optstring_len; i++){
	        if(caracter == optstring[i]){ //found the expected argument
	            optind++;
	            
	            return caracter;
	        }
	    }
	}
	printf("\ninvalid option -- '%c'\n", caracter);
	return -1;
    
}