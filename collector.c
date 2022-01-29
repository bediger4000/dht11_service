#include <stdio.h>
#include <string.h> /* strerror() */
#include <unistd.h> /* close(2) */
#include <stdlib.h> /* atoi */
#include <sys/socket.h> /* inet_aton */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "dht_service.h"

int initialize_socket(in_addr_t server, uint16_t network_endian_port);
char *format_data(void);

int skt;
struct sockaddr_in servaddr;
ssize_t n;
socklen_t slen;

void
collect_data(in_addr_t server, uint16_t network_endian_port, int report_interval)
{
	int consecutive_errors = 0;

	if (0 > initialize_socket(server, network_endian_port)) {
		return;
	}

	while (1) {

		char *message = format_data();

		logmsg("sending %d bytes", strlen(message));

		n = sendto(
			skt,
			message, strlen(message),
			MSG_CONFIRM|MSG_DONTWAIT,
			(const struct sockaddr *)&servaddr, sizeof(servaddr)
		);
		if (0 > n) {
			logmsg("sendto problem: %s\n", strerror(errno));
			++consecutive_errors;
		} else if (n != (ssize_t)strlen(message)) {
			logmsg("sent %d bytes, wanted to send %d\n", n, strlen(message));
			++consecutive_errors;
		} else {
			consecutive_errors = 0;
		}

		if (consecutive_errors > 4) {
			logmsg("leaving collection loop on too many errors");
			break;
		}

		sleep(report_interval);
	}
}

int
initialize_socket(in_addr_t server, uint16_t network_endian_port)
{
	if (0 > (skt = socket(AF_INET, SOCK_DGRAM, 0))) {
		logmsg("problem creating UDP socket: %s", strerror(errno));
		return -1;
	}

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = network_endian_port;
	servaddr.sin_addr.s_addr = server;
	slen = sizeof(servaddr);

	return 0;
}

char message_buffer[256];

char *
format_data(void)
{
	float humidity = 0.0, temperature = 0.0, temp2 = 0.0;
	int pin = 17;

	read_dht11(pin, &temperature, &humidity);

	ds18b20_temp(&temp2);

	message_buffer[0] = '\0';

	sprintf(&message_buffer[0], "%.03f\t%.03f\t%.03f", temperature, humidity, temp2);

	return &message_buffer[0];
}
