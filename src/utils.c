#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


void die(char *fmt, ...)
{
        va_list arglist;

        va_start(arglist, fmt);
        fprintf(stderr, "ERROR: ");
        vfprintf(stderr, fmt, arglist);
        fprintf(stderr, "\n");
        va_end(arglist);
        
        exit(2);
}
