#include <stdio.h>
#include <netinet/in.h> /* in_addr_t */

#include "dht_service.h"

int
main(int ac, char **av)
{
	float temp;

	if (0 == ds18b20_temp(&temp)) {
		printf("%.03f\n", temp);
	}

	return 0;
}
