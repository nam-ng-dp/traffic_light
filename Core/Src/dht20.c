#include "dht20.h"
#include "global.h"
#include "software_timer.h"
#include <stddef.h>

#ifndef UNIT_TEST
#include "main.h"
extern I2C_HandleTypeDef hi2c1;
#endif

#define DHT20_I2C_ADDR       (0x38u << 1)
#define DHT20_TIMER_CH       2
#define DHT20_I2C_TIMEOUT_MS 2

static uint8_t dht20_state = DHT20_STATE_INIT;

void dht20_init(void) {
	dht20_state = DHT20_STATE_INIT;
	clearTimer(DHT20_TIMER_CH);
}

uint8_t dht20_crc8(const uint8_t *data, uint8_t len) {
	uint8_t crc = 0xFFu;
	if (data == NULL) {
		return crc;
	}
	for (uint8_t i = 0; i < len; i++) {
		crc ^= data[i];
		for (uint8_t bit = 0; bit < 8; bit++) {
			if (crc & 0x80u) {
				crc = (uint8_t)((crc << 1) ^ 0x31u);
			} else {
				crc = (uint8_t)(crc << 1);
			}
		}
	}
	return crc;
}

int dht20_parse_raw(const uint8_t raw[7], int *out_temp, int *out_humi) {
	if (raw == NULL) {
		return 0;
	}
	/* Bit 7 of status byte must be 0 (not busy) and CRC-8 must match byte 6 */
	if ((raw[0] & 0x80u) != 0u) {
		return 0;
	}
	if (dht20_crc8(raw, 6) != raw[6]) {
		return 0;
	}

	uint32_t s_rh = ((uint32_t)raw[1] << 12) |
	                ((uint32_t)raw[2] << 4)  |
	                ((uint32_t)raw[3] >> 4);
	uint32_t s_t  = (((uint32_t)raw[3] & 0x0Fu) << 16) |
	                ((uint32_t)raw[4] << 8)            |
	                (uint32_t)raw[5];

	int humi = (int)((s_rh * 100ULL) >> 20);
	int temp = (int)((s_t * 200ULL) >> 20) - 50;

	if (out_temp != NULL) {
		*out_temp = temp;
	}
	if (out_humi != NULL) {
		*out_humi = humi;
	}
	return 1;
}

void dht20_read(void) {
	switch (dht20_state) {
	case DHT20_STATE_INIT: {
#ifndef UNIT_TEST
		uint8_t cmd = 0x71u;
		uint8_t status_byte = 0u;
		if (HAL_I2C_Master_Transmit(&hi2c1, DHT20_I2C_ADDR, &cmd, 1, DHT20_I2C_TIMEOUT_MS) == HAL_OK) {
			HAL_I2C_Master_Receive(&hi2c1, DHT20_I2C_ADDR, &status_byte, 1, DHT20_I2C_TIMEOUT_MS);
		}
		(void)status_byte;
#endif
		setTimer(DHT20_TIMER_CH, 100);
		dht20_state = DHT20_STATE_TRIGGER;
		break;
	}

	case DHT20_STATE_IDLE:
		if (timer_flag[DHT20_TIMER_CH] == 1) {
			dht20_state = DHT20_STATE_TRIGGER;
		}
		break;

	case DHT20_STATE_TRIGGER: {
#ifndef UNIT_TEST
		uint8_t trig_cmd[3] = { 0xACu, 0x33u, 0x00u };
		HAL_I2C_Master_Transmit(&hi2c1, DHT20_I2C_ADDR, trig_cmd, 3, DHT20_I2C_TIMEOUT_MS);
#endif
		setTimer(DHT20_TIMER_CH, 100); /* >= 80 ms datasheet conversion wait */
		dht20_state = DHT20_STATE_WAIT;
		break;
	}

	case DHT20_STATE_WAIT:
		if (timer_flag[DHT20_TIMER_CH] == 1) {
			dht20_state = DHT20_STATE_READ;
		}
		break;

	case DHT20_STATE_READ: {
#ifndef UNIT_TEST
		uint8_t raw[7] = { 0 };
		if (HAL_I2C_Master_Receive(&hi2c1, DHT20_I2C_ADDR, raw, 7, DHT20_I2C_TIMEOUT_MS) == HAL_OK) {
			int t = 0;
			int h = 0;
			if (dht20_parse_raw(raw, &t, &h)) {
				temperature = t;
				humidity = h;
			}
		}
#endif
		setTimer(DHT20_TIMER_CH, 2000);
		dht20_state = DHT20_STATE_IDLE;
		break;
	}

	default:
		dht20_state = DHT20_STATE_INIT;
		break;
	}
}

int dht20_get_temperature(void) {
	return temperature;
}

int dht20_get_humidity(void) {
	return humidity;
}
