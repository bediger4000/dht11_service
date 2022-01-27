#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h> /* in_addr_t */

#include "common_dht_read.h"
#include "pi_2_dht_read.h"
#include "dht_service.h"

int
read_dht11(int pin, float *temperature, float *humidity)
{
	int sensor = DHT11;
	int good_reads = 0;
	int loop_count = 0;

	while (loop_count < 10 && good_reads < 3) {
		++loop_count;

		*humidity = 0; *temperature = 0;

		int result = pi_2_dht_read(sensor, pin, humidity, temperature);

		if (result != 0) {
			switch (result) {
			case DHT_ERROR_TIMEOUT:
				fprintf(stderr, "%s DHT read timeout\n", timestamp());
				break;
			case DHT_ERROR_CHECKSUM:
				fprintf(stderr, "%s DHT read checksum error\n", timestamp());
				break;
			case DHT_ERROR_ARGUMENT:
				fprintf(stderr, "%s DHT argument problem\n", timestamp());
				break;
			case DHT_ERROR_GPIO :
				fprintf(stderr, "%s GPIO error\n", timestamp());
				break;
			default:
				fprintf(stderr, "%s Unknown error 0x%x\n", timestamp(), result);
			}
			sleep(1);
			continue;
		}
		++good_reads;
		sleep(1);
	}

	if (loop_count >= 10) {
		return -1;
	}

	return 0;
}
