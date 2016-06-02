/*
 * i2c_lcd.c
 *
 *  Created on: 2. 6. 2016
 *      Author: chrom.rade
 */
#include "stm32f1xx_hal.h"
#include "i2c_lcd.h"

#include <string.h>

extern I2C_HandleTypeDef hi2c1;


void I2C_LiquidCrystal_print(struct LiquidCrystal_I2C *lcd, const char * text) {
	unsigned int size = strlen(text);
	for (int i = 0; i < size; ++i) {
		I2C_LiquidCrystal_write(lcd, text[i]);
	}
}

void I2C_LiquidCrystal_create(struct LiquidCrystal_I2C *lcd, uint16_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) {
	lcd->_Addr = lcd_Addr;
	lcd->_cols = lcd_cols;
	lcd->_rows = lcd_rows;
	lcd->_backlightval = LCD_NOBACKLIGHT;
}

void I2C_liquidCrystal_init(struct LiquidCrystal_I2C *lcd) {
	lcd->_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	I2C_LiquidCrystal_begin(lcd, lcd->_cols, lcd->_rows, LCD_5x8DOTS);
}


void I2C_LiquidCrystal_write(struct LiquidCrystal_I2C *lcd, uint8_t value) {
	I2C_LiquidCrystal_send(lcd, value, Rs);
}


void I2C_LiquidCrystal_begin(struct LiquidCrystal_I2C *lcd, uint8_t cols, uint8_t lines, uint8_t dotsize) {
	if (lines > 1) {
		lcd->_displayfunction |= LCD_2LINE;
	}
	lcd->_numlines = lines;

	if ((dotsize != 0) && (lines == 1)) {
		lcd->_displayfunction |= LCD_5x10DOTS;
	}

	Serialprintln("01");
	HAL_Delay(50);

	Serialprintln("02");
	I2C_LiquidCrystal_expanderWrite(lcd, lcd->_backlightval);

	Serialprintln("03");
	HAL_Delay(1000);

	Serialprintln("04");
	I2C_LiquidCrystal_write4bits(lcd, 0x03 << 4);

	Serialprintln("05");
	HAL_Delay(5);

	Serialprintln("06");
	I2C_LiquidCrystal_write4bits(lcd, 0x03 << 4);

	Serialprintln("07");
	HAL_Delay(5);


	Serialprintln("08");
	I2C_LiquidCrystal_write4bits(lcd, 0x03 << 4);

	Serialprintln("09");
	HAL_Delay(1);

	Serialprintln("10");
	I2C_LiquidCrystal_write4bits(lcd, 0x02 << 4);


	Serialprintln("11");

	I2C_LiquidCrystal_command(lcd, LCD_FUNCTIONSET | lcd->_displayfunction);

	lcd->_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	I2C_LiquidCrystal_display(lcd);

	I2C_LiquidCrystal_clear(lcd);

	lcd->_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	I2C_LiquidCrystal_command(lcd, LCD_ENTRYMODESET | lcd->_displaymode);

	I2C_LiquidCrystal_home(lcd);
}

void  I2C_LiquidCrystal_clear(struct LiquidCrystal_I2C *lcd) {
	I2C_LiquidCrystal_command(lcd, LCD_CLEARDISPLAY);
	HAL_Delay(2);
}

void  I2C_LiquidCrystal_home(struct LiquidCrystal_I2C *lcd) {
	I2C_LiquidCrystal_command(lcd, LCD_RETURNHOME);
	HAL_Delay(2);
}

void  I2C_LiquidCrystal_setCursor(struct LiquidCrystal_I2C *lcd, uint8_t col, uint8_t row) {
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > lcd->_numlines ) {
		row = lcd->_numlines - 1;
	}
	I2C_LiquidCrystal_command(lcd, LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void  I2C_LiquidCrystal_noDisplay(struct LiquidCrystal_I2C *lcd) {
	lcd->_displaycontrol &= ~LCD_DISPLAYON;
	I2C_LiquidCrystal_command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}

void  I2C_LiquidCrystal_display(struct LiquidCrystal_I2C *lcd) {
	lcd->_displaycontrol |= LCD_DISPLAYON;
	I2C_LiquidCrystal_command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}

void  I2C_LiquidCrystal_noCursor(struct LiquidCrystal_I2C *lcd) {
	lcd->_displaycontrol &= ~LCD_CURSORON;
	I2C_LiquidCrystal_command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}

void  I2C_LiquidCrystal_cursor(struct LiquidCrystal_I2C *lcd) {
	lcd->_displaycontrol |= LCD_CURSORON;
	I2C_LiquidCrystal_command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}

void  I2C_LiquidCrystal_noBlink(struct LiquidCrystal_I2C *lcd) {
	lcd->_displaycontrol &= ~LCD_BLINKON;
	I2C_LiquidCrystal_command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}

void  I2C_LiquidCrystal_blink(struct LiquidCrystal_I2C *lcd) {
	lcd->_displaycontrol |= LCD_BLINKON;
	I2C_LiquidCrystal_command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}

void  I2C_LiquidCrystal_scrollDisplayLeft(struct LiquidCrystal_I2C *lcd) {
	I2C_LiquidCrystal_command(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void  I2C_LiquidCrystal_scrollDisplayRight(struct LiquidCrystal_I2C *lcd) {
	I2C_LiquidCrystal_command(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void I2C_LiquidCrystal_rightToRight(struct LiquidCrystal_I2C *lcd) {
	lcd->_displaymode |= ~LCD_ENTRYLEFT;
	I2C_LiquidCrystal_command(lcd, LCD_ENTRYMODESET | lcd->_displaymode);
}


void I2C_LiquidCrystal_rightToLeft(struct LiquidCrystal_I2C *lcd) {
	lcd->_displaymode &= ~LCD_ENTRYLEFT;
	I2C_LiquidCrystal_command(lcd, LCD_ENTRYMODESET | lcd->_displaymode);
}

void I2C_LiquidCrystal_autoscroll(struct LiquidCrystal_I2C *lcd) {
	lcd->_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	I2C_LiquidCrystal_command(lcd, LCD_ENTRYMODESET | lcd->_displaymode);
}

void I2C_LiquidCrystal_noAutoscroll(struct LiquidCrystal_I2C *lcd) {
	lcd->_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	I2C_LiquidCrystal_command(lcd, LCD_ENTRYMODESET | lcd->_displaymode);
}

void I2C_LiquidCrystal_createChar(struct LiquidCrystal_I2C *lcd, uint8_t location, uint8_t charmap[]) {
	location &= 0x7;
	I2C_LiquidCrystal_command(lcd, LCD_SETCGRAMADDR | (location << 3));
	for (int i = 0; i < 8; ++i) {
		I2C_LiquidCrystal_write(lcd, charmap[i]);
	}
}

void I2C_LiquidCrystal_noBacklight(struct LiquidCrystal_I2C *lcd) {
	lcd->_backlightval = LCD_NOBACKLIGHT;
	I2C_LiquidCrystal_expanderWrite(lcd, 0);

}

void I2C_LiquidCrystal_backlight(struct LiquidCrystal_I2C *lcd) {
	lcd->_backlightval = LCD_BACKLIGHT;
	I2C_LiquidCrystal_expanderWrite(lcd, 0);
}

void I2C_LiquidCrystal_command(struct LiquidCrystal_I2C *lcd, uint8_t value) {
	I2C_LiquidCrystal_send(lcd, value, 0);
}

void I2C_LiquidCrystal_send(struct LiquidCrystal_I2C *lcd, uint8_t value, uint8_t mode) {
	uint8_t highnib = value & 0xf0;
	uint8_t lownib = (value << 4) & 0xf0;
	I2C_LiquidCrystal_write4bits(lcd, (highnib) | mode);
	I2C_LiquidCrystal_write4bits(lcd, (lownib) | mode);
}

void I2C_LiquidCrystal_write4bits(struct LiquidCrystal_I2C *lcd, uint8_t value) {
	I2C_LiquidCrystal_expanderWrite(lcd, value);
	I2C_LiquidCrystal_pulseEnable(lcd, value);
}

void I2C_LiquidCrystal_expanderWrite(struct LiquidCrystal_I2C *lcd, uint8_t _data) {
	uint8_t data = _data | lcd->_backlightval;
	HAL_I2C_Master_Transmit(&hi2c1, lcd->_Addr, &data, sizeof(_data), LCD_TIMEOUT);
}

void I2C_LiquidCrystal_pulseEnable(struct LiquidCrystal_I2C *lcd, uint8_t _data) {
	I2C_LiquidCrystal_expanderWrite(lcd, _data | En);
	HAL_Delay(1);
	I2C_LiquidCrystal_expanderWrite(lcd, _data & ~En);
	HAL_Delay(50);
}


