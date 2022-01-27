#include <time.h> /* strftime */
#include "dht_service.h"

char buffer[128];

char *
timestamp(void)
{
	struct tm *tm;
	time_t t, dummy;

	t = time(&dummy);
	tm = localtime(&t);
	
	strftime(buffer, sizeof(buffer), "%FT%H:%M:%S%z", tm);

	return &buffer[0];
}
