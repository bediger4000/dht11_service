#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h> /* strftime */
#include <netinet/in.h>
#include "dht_service.h"

void
logmsg(const char *format, ...)
{
	va_list ap;
	size_t fmtlen;
	char* tstmp;
	char *newfmt;

	tstmp = timestamp();

	fmtlen = strlen(format) + 28;
	newfmt = calloc(fmtlen, 1);
	sprintf(newfmt, "%s\t%s\n", tstmp, format);

	va_start(ap, format);

	vfprintf(stderr, newfmt, ap);

	va_end(ap);
}
