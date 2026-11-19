#ifndef INC_LCD_H_
#define INC_LCD_H_

#define LCD_STATE_ROW0 0
#define LCD_STATE_ROW1 1

void lcd_init(void);
void lcd_clear(void);
void lcd_goto(int row, int col);
void lcd_print_str(const char *str);
void lcd_print_int(int num);
void lcd_update_auto(int count1, int count2);
void lcd_update_manual(int phase_id);
void lcd_update_setting(int color_mode, int val);
void lcd_show_boot_msg(void);
void lcd_clear_boot_msg(void);
void lcd_update(void);
const char* lcd_get_line(int row);

#endif /* INC_LCD_H_ */
