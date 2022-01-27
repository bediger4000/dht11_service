#include <stdio.h>
#include "dht_service.h"
#include "common_dht_read.h"
#include "pi_2_dht_read.h"

int
main(int ac, char **av)
{
	float humidity = 0, temperature = 0;
	int pin = 17;

	if (0 == read_dht11(pin, &temperature, &humidity)) {
		printf("%s %.02f%%, %.02fC\n", timestamp(), humidity, temperature);
	}

	return 0;
}
