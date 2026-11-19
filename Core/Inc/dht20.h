#ifndef INC_DHT20_H_
#define INC_DHT20_H_

#include <stdint.h>

#define DHT20_STATE_INIT     0
#define DHT20_STATE_IDLE     1
#define DHT20_STATE_TRIGGER  2
#define DHT20_STATE_WAIT     3
#define DHT20_STATE_READ     4

void dht20_init(void);
void dht20_read(void);
int  dht20_get_temperature(void);
int  dht20_get_humidity(void);
uint8_t dht20_crc8(const uint8_t *data, uint8_t len);
int  dht20_parse_raw(const uint8_t raw[7], int *out_temp, int *out_humi);

#endif /* INC_DHT20_H_ */
