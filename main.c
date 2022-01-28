#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h> /* in_addr_t */
#include <arpa/inet.h>  /* htons() */

#include "dht_service.h"

void usage(char *progname);

int
main(int argc, char **argv)
{
	char *intervalstring = "120";
	char *portstring = "7896";
	char *server_address = "10.0.0.1";

	in_addr_t server;
	uint16_t port;
	int interval;

	int opt;
	while (0 < (opt = getopt(argc, argv, "i:p:s:x"))) {
		switch (opt) {
		case 'i':
			intervalstring = optarg;
			break;
		case 'p':
			portstring = optarg;
			break;
		case 's':
			server_address = optarg;
			break;
		case 'x':
			usage(argv[0]);
			exit(0);
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}

	interval = atoi(intervalstring);
	port = htons(atoi(portstring));
	server = find_address(server_address);

	if (interval < 0 || server == 0 || port == 0) {
		fprintf(stderr, "Interval of %d secs or\n", interval);
		fprintf(stderr, "UDP port of %d secs or\n", ntohs(port));
		fprintf(stderr, "host of %s is wrong\n", server_address);
		return -1;
	}

	printf("Contacting %d.%d.%d.%d:%d at %d sec intervals\n",
		server&0xff,
		(server>>8)&0xff,
		(server>>16)&0xff,
		(server>>24)&0xff,
		ntohs(port),
		interval
	);
		

	return 0;
}

void
usage(char *progname)
{
	fprintf(stderr, "%s: temperature and humidity collector\n", progname);
	fprintf(stderr, "Usage: %s <-i interval> <-p server port> <-s server>\n", progname);
	fprintf(stderr, "Flags:\n"
		"\t-i interval between reports in seconds\n"
		"\t-p server's UDP port\n"
		"\t-s server's name or IP address\n"
	);
}
