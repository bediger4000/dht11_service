#include <string.h> /* strerror() */
#include <sys/socket.h> /* inet_aton */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>

#include "dht_service.h"

in_addr_t
find_address(char *server)
{
	struct in_addr inp;

	if (0 == inet_aton(server, &inp)) {
		struct hostent *hent;

		if (NULL == (hent = gethostbyname(server))) {
			fprintf(stderr, "%s %s: %s\n", timestamp(), server,
				hstrerror(h_errno));
			return 0;
		}

		return (in_addr_t)hent->h_addr_list[0];
	}

	return inp.s_addr;
}
