CFLAGS=-Wall -Wextra -g

OBJFILES = work.o timestamp.o pi_2_mmio.o pi_2_dht_read.o

all: $(OBJFILES)

work.o: work.c dht_service.h common_dht_read.h pi_2_dht_read.h
	$(CC) $(CFLAGS) -c work.c

timestamp.o: timestamp.c dht_service.h 
	$(CC) $(CFLAGS) -c timestamp.c

pi_2_mmio.o: pi_2_mmio.c pi_2_mmio.h
	$(CC) $(CFLAGS) -c pi_2_mmio.c

pi_2_dht_read.o: pi_2_dht_read.c pi_2_dht_read.h pi_2_mmio.h
	$(CC) $(CFLAGS) -c pi_2_dht_read.c

clean:
	-rm -rf $(OBJFILES)
