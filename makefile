CFLAGS=-Wall -Wextra -g

OBJFILES = work.o timestamp.o pi_2_mmio.o pi_2_dht_read.o \
	common_dht_read.o temp2.o address.o collector.o logging.o

all: dht_service

dht_service: main.o $(OBJFILES)
	$(CC) $(CFLAGS) -o dht_service main.o $(OBJFILES)

check: check.c $(OBJFILES)
	$(CC) $(CFLAGS) -o check check.c $(OBJFILES)

checkt: checkt.c timestamp.o temp2.o dht_service.h
	$(CC) $(CFLAGS) -o checkt checkt.c timestamp.o temp2.o

main.o: main.c dht_service.h
	$(CC) $(CFLAGS) -c main.c

logging.o: logging.c dht_service.h
	$(CC) $(CFLAGS) -c logging.c

collector.o: collector.c dht_service.h
	$(CC) $(CFLAGS) -c collector.c

work.o: work.c dht_service.h common_dht_read.h pi_2_dht_read.h
	$(CC) $(CFLAGS) -c work.c

timestamp.o: timestamp.c dht_service.h 
	$(CC) $(CFLAGS) -c timestamp.c

address.o: address.c dht_service.h 
	$(CC) $(CFLAGS) -c address.c

temp2.o: temp2.c dht_service.h
	$(CC) $(CFLAGS) -c temp2.c

pi_2_mmio.o: pi_2_mmio.c pi_2_mmio.h
	$(CC) $(CFLAGS) -c pi_2_mmio.c

pi_2_dht_read.o: pi_2_dht_read.c pi_2_dht_read.h pi_2_mmio.h
	$(CC) $(CFLAGS) -c pi_2_dht_read.c

common_dht_read.o: common_dht_read.c common_dht_read.h
	$(CC) $(CFLAGS) -c common_dht_read.c 

clean:
	-rm -rf $(OBJFILES)
	-rm -rf check checkt dht_service
