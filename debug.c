
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

DEBUG_ST *
dbginit(char *v)
{
	char *c = v;
	DEBUG_ST *d = (DEBUG_ST *)malloc(sizeof(DEBUG_ST));
	d->level = 0;
	while(*c)
		if(*c++ == VERBOSE_CHAR)
			d->level++;
	return d;
}

void 
dbgprintf(DEBUG_ST *d,int verbose,const char *fmt, ...)
{
	va_list ap;
	if(verbose > d->level )
		return;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}


void 
dbgend(DEBUG_ST *d)
{
	free(d);
}
