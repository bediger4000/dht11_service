int read_dht11(int pin, float *temperature, float *humidity);
char *timestamp(void);
int ds18b20_temp(float *temperature);
in_addr_t find_address(char *server);
void collect_data(in_addr_t server, uint16_t network_endian_port, int report_interval);

