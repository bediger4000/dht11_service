CFLAGS=-Wall -Wextra -g

OBJFILES = work.o timestamp.o

all: $(OBJFILES)

work.o: work.c dht_service.h common_dht_read.h pi_2_dht_read.h
	$(CC) $(CFLAGS) -c work.c

timestamp.o: timestamp.c dht_service.h 
	$(CC) $(CFLAGS) -c timestamp.c

clean:
	-rm -rf $(OBJFILES)
