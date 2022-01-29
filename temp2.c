#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> /* opendir */
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h> /* in_addr_t */
#include "dht_service.h"

char *thermometer_filename(void);
int parse_buffer(char *buffer, size_t bufsz, float *temperature);

#define DEVDIR "/sys/bus/w1/devices"
char filename[1024];

int
ds18b20_temp(float *temperature)
{
	char *filename;
	FILE *fin;
	size_t bufsz;
	char buffer[128];

	*temperature = 0.0;

	if (NULL == (filename = thermometer_filename())) {
		logmsg("couldn't find termometer file name");
		return -1;
	}

	logmsg("thermometer %s", filename);

	if (NULL == (fin = fopen(filename, "r"))) {
		logmsg("problem opening termometer file \"%s\": %s",
			filename,
			strerror(errno)
		);
		return -1;
	}

	if (0 == (bufsz = fread(&buffer[0], 1, sizeof(buffer), fin))) {
		logmsg("problem reading termometer file \"%s\": %s",
			filename,
			strerror(errno)
		);
		return -1;
	}

	fclose(fin);

	return parse_buffer(buffer, bufsz, temperature);
}

int
parse_buffer(char *buffer, size_t bufsz, float *temperature)
{
	char *line = buffer;

	*temperature = 0.0;

	for (int i = 0; i < (int)bufsz; ++i) {
		if (buffer[i] == '\n') {
			int itemp;
			int linelen = (int)(&buffer[i] - line);
			char *tat, *eqat;

			if (linelen < 7) {
				line = &buffer[i+1];
				continue;
			}

			if (NULL == (tat = index(line, 't'))) {
				line = &buffer[i+1];
				continue;
			}
			if (NULL == (eqat = index(tat, '='))) {
				line = &buffer[i+1];
				continue;
			}

			buffer[i] = '\0';
			itemp = atoi(eqat+1);
			*temperature = (float)itemp/1000.0;
			return 0;
		}
	}

	return -1;
}

char *
thermometer_filename(void)
{
	DIR *dir;
	struct dirent *entry;
	char name[56];

	name[0] = '\0';
	filename[0] = '\0';

	dir = opendir(DEVDIR);
	if (!dir) {
		logmsg("problem opening %s: %s\n", DEVDIR, strerror(errno));
		return NULL;
	}

	while (NULL != (entry = readdir(dir))) {
		if (strncmp("28-", entry->d_name, 3) == 0) {
			strcpy(name, entry->d_name);
		}
	}

	if (strlen(name) > 0) {
		sprintf(filename, "/sys/bus/w1/devices/%s/w1_slave", name);
	}

	closedir(dir);

	return filename;
}
