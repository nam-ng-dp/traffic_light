#include "lcd.h"
#include "global.h"
#include "traffic_light.h"
#include <stdio.h>
#include <string.h>

#ifndef UNIT_TEST
#include "main.h"
extern I2C_HandleTypeDef hi2c1;
#endif

#define LCD_ROWS            2
#define LCD_COLS            16
#define LCD_I2C_TIMEOUT_MS  2

/* PCF8574 -> HD44780 bit mapping */
#define LCD_PIN_RS          0x01u
#define LCD_PIN_EN          0x04u
#define LCD_PIN_BL          0x08u

static char lcd_buf[LCD_ROWS][LCD_COLS + 1];
static uint8_t row_dirty[LCD_ROWS] = { 0, 0 };
static uint8_t boot_msg_active = 0;
static int cursor_row = 0;
static int cursor_col = 0;
static uint8_t lcd_i2c_addr = 0;
static uint8_t lcd_state = LCD_STATE_ROW0;

static void lcd_set_line(int row, const char *text) {
	if (row < 0 || row >= LCD_ROWS || text == NULL) {
		return;
	}
	char formatted[LCD_COLS + 1];
	size_t len = strlen(text);
	for (int i = 0; i < LCD_COLS; i++) {
		formatted[i] = ((size_t)i < len) ? text[i] : ' ';
	}
	formatted[LCD_COLS] = '\0';

	if (memcmp(lcd_buf[row], formatted, LCD_COLS) != 0) {
		memcpy(lcd_buf[row], formatted, LCD_COLS + 1);
		row_dirty[row] = 1;
	}
}

static void lcd_format_env_line(char *out, size_t out_size) {
	snprintf(out, out_size, "T:%02dC   H:%02d%%   ", temperature, humidity);
}

#ifndef UNIT_TEST
static HAL_StatusTypeDef lcd_write_nibble(uint8_t nibble_hi, uint8_t rs) {
	if (lcd_i2c_addr == 0u) {
		return HAL_ERROR;
	}
	uint8_t base = (uint8_t)((nibble_hi & 0xF0u) | LCD_PIN_BL | (rs ? LCD_PIN_RS : 0u));
	uint8_t seq[2] = {
		(uint8_t)(base | LCD_PIN_EN),
		base
	};
	return HAL_I2C_Master_Transmit(&hi2c1, lcd_i2c_addr, seq, 2, LCD_I2C_TIMEOUT_MS);
}

static HAL_StatusTypeDef lcd_write_byte(uint8_t val, uint8_t rs) {
	if (lcd_i2c_addr == 0u) {
		return HAL_ERROR;
	}
	uint8_t ctrl = (uint8_t)(LCD_PIN_BL | (rs ? LCD_PIN_RS : 0u));
	uint8_t hi = (uint8_t)((val & 0xF0u) | ctrl);
	uint8_t lo = (uint8_t)(((val << 4) & 0xF0u) | ctrl);
	uint8_t seq[4] = {
		(uint8_t)(hi | LCD_PIN_EN),
		hi,
		(uint8_t)(lo | LCD_PIN_EN),
		lo
	};
	return HAL_I2C_Master_Transmit(&hi2c1, lcd_i2c_addr, seq, 4, LCD_I2C_TIMEOUT_MS);
}
#endif

void lcd_init(void) {
	for (int r = 0; r < LCD_ROWS; r++) {
		memset(lcd_buf[r], ' ', LCD_COLS);
		lcd_buf[r][LCD_COLS] = '\0';
		row_dirty[r] = 0;
	}
	boot_msg_active = 0;
	cursor_row = 0;
	cursor_col = 0;
	lcd_state = LCD_STATE_ROW0;
	lcd_i2c_addr = 0;

#ifndef UNIT_TEST
	static const uint8_t candidates[4] = { 0x27u, 0x3Fu, 0x3Eu, 0x20u };
	for (size_t i = 0; i < sizeof(candidates); i++) {
		uint16_t addr8 = (uint16_t)(candidates[i] << 1);
		if (HAL_I2C_IsDeviceReady(&hi2c1, addr8, 1, 5) == HAL_OK) {
			lcd_i2c_addr = (uint8_t)addr8;
			break;
		}
	}

	if (lcd_i2c_addr != 0u) {
		/* Startup-only HD44780 4-bit initialization sequence */
		HAL_Delay(20);
		lcd_write_nibble(0x30u, 0);
		HAL_Delay(5);
		lcd_write_nibble(0x30u, 0);
		HAL_Delay(1);
		lcd_write_nibble(0x30u, 0);
		HAL_Delay(1);
		lcd_write_nibble(0x20u, 0);
		HAL_Delay(1);

		lcd_write_byte(0x28u, 0); /* 4-bit, 2 lines, 5x8 font */
		lcd_write_byte(0x0Cu, 0); /* Display ON, cursor OFF */
		lcd_write_byte(0x06u, 0); /* Entry mode: increment */
		lcd_write_byte(0x01u, 0); /* Clear display */
		HAL_Delay(2);
	}
#endif
}

void lcd_clear(void) {
	for (int r = 0; r < LCD_ROWS; r++) {
		lcd_set_line(r, "");
	}
	cursor_row = 0;
	cursor_col = 0;
}

void lcd_goto(int row, int col) {
	if (row < 0) {
		row = 0;
	} else if (row >= LCD_ROWS) {
		row = LCD_ROWS - 1;
	}
	if (col < 0) {
		col = 0;
	} else if (col >= LCD_COLS) {
		col = LCD_COLS - 1;
	}
	cursor_row = row;
	cursor_col = col;
}

void lcd_print_str(const char *str) {
	if (str == NULL) {
		return;
	}
	while (*str != '\0' && cursor_col < LCD_COLS) {
		if (lcd_buf[cursor_row][cursor_col] != *str) {
			lcd_buf[cursor_row][cursor_col] = *str;
			row_dirty[cursor_row] = 1;
		}
		cursor_col++;
		str++;
	}
}

void lcd_print_int(int num) {
	char tmp[17];
	snprintf(tmp, sizeof(tmp), "%d", num);
	lcd_print_str(tmp);
}

void lcd_show_boot_msg(void) {
	lcd_set_line(0, "XNUCLEO-F103RB  ");
	lcd_set_line(1, "TRAFFIC LIGHT   ");
	boot_msg_active = 1;
}

void lcd_clear_boot_msg(void) {
	boot_msg_active = 0;
	row_dirty[0] = 1;
	row_dirty[1] = 1;
}

void lcd_update_auto(int count1, int count2) {
	if (boot_msg_active) {
		return;
	}
	char line0[32];
	char line1[32];
	snprintf(line0, sizeof(line0), "AUTO 1:%02ds 2:%02ds", count1, count2);
	lcd_format_env_line(line1, sizeof(line1));
	lcd_set_line(0, line0);
	lcd_set_line(1, line1);
}

void lcd_update_manual(int phase_id) {
	if (boot_msg_active) {
		return;
	}
	int p = phase_id;
	if (p >= STATUS_MAN_RED_GREEN && p <= STATUS_MAN_YELLOW_RED) {
		p -= (STATUS_MAN_RED_GREEN - 1);
	}
	const char *phase_str = "-----";
	switch (p) {
	case 1:
		phase_str = "R1-G2";
		break;
	case 2:
		phase_str = "R1-Y2";
		break;
	case 3:
		phase_str = "G1-R2";
		break;
	case 4:
		phase_str = "Y1-R2";
		break;
	default:
		break;
	}
	char line0[32];
	char line1[32];
	snprintf(line0, sizeof(line0), "MAN: P%d [%s]", p, phase_str);
	lcd_format_env_line(line1, sizeof(line1));
	lcd_set_line(0, line0);
	lcd_set_line(1, line1);
}

void lcd_update_setting(int color_mode, int val) {
	if (boot_msg_active) {
		return;
	}
	const char *mode_str = "MODE";
	if (color_mode == STATUS_SET_RED || color_mode == TRAFFIC_RED) {
		mode_str = "RED";
	} else if (color_mode == STATUS_SET_AMBER || color_mode == TRAFFIC_AMBER) {
		mode_str = "AMBER";
	} else if (color_mode == STATUS_SET_GREEN || color_mode == TRAFFIC_GREEN) {
		mode_str = "GREEN";
	}
	char line0[32];
	snprintf(line0, sizeof(line0), "SET %-5s: %02ds  ", mode_str, val);
	lcd_set_line(0, line0);
	lcd_set_line(1, "B2:+ B3:- B4:OK ");
}

static int lcd_is_row_dirty(int row) {
	if (row < 0 || row >= LCD_ROWS) {
		return 0;
	}
	return row_dirty[row] != 0;
}

static void lcd_hw_flush_row(int row) {
	if (row < 0 || row >= LCD_ROWS) {
		return;
	}
	row_dirty[row] = 0;
#ifndef UNIT_TEST
	if (lcd_i2c_addr != 0u) {
		uint8_t ddram_cmd = (uint8_t)(0x80u | (row == 0 ? 0x00u : 0x40u));
		if (lcd_write_byte(ddram_cmd, 0) == HAL_OK) {
			for (int col = 0; col < LCD_COLS; col++) {
				if (lcd_write_byte((uint8_t)lcd_buf[row][col], 1) != HAL_OK) {
					break;
				}
			}
		}
	}
#endif
}

void lcd_update(void) {
	switch (lcd_state) {
	case LCD_STATE_ROW0:
		if (lcd_is_row_dirty(0)) {
			lcd_hw_flush_row(0);
			lcd_state = LCD_STATE_ROW1;
			break;
		}
		lcd_state = LCD_STATE_ROW1;
		break;

	case LCD_STATE_ROW1:
		if (lcd_is_row_dirty(1)) {
			lcd_hw_flush_row(1);
			lcd_state = LCD_STATE_ROW0;
			break;
		}
		lcd_state = LCD_STATE_ROW0;
		break;

	default:
		break;
	}
}

const char* lcd_get_line(int row) {
	if (row < 0 || row >= LCD_ROWS) {
		return "";
	}
	return lcd_buf[row];
}
