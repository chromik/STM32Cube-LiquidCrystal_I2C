/*
 * i2c_lcd.h
 *
 *  Created on: 2. 6. 2016
 *      Author: chrom.rade
 */

#ifndef I2C_LCD_I2C_LCD_H_
#define I2C_LCD_I2C_LCD_H_



#define LCD_TIMEOUT 100
// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

struct LiquidCrystal_I2C {
	uint16_t _Addr;
	uint32_t _Timeout;

	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;
	uint8_t _numlines;
	uint8_t _cols;
	uint8_t _rows;
	uint8_t _backlightval;
};
/*
 * i2c_lcd.c
 *
 *  Created on: 2. 6. 2016
 *      Author: chrom.rade
 */
#include "stm32f1xx_hal.h"
#include "i2c_lcd.h"

extern I2C_HandleTypeDef hi2c1;

void I2C_LiquidCrystal_print(struct LiquidCrystal_I2C *lcd, const char * text);

void I2C_LiquidCrystal_init(struct LiquidCrystal_I2C *lcd, uint16_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows);

void I2C_LiquidCrystal_begin(struct LiquidCrystal_I2C *lcd, uint8_t cols, uint8_t lines, uint8_t dotsize);

void  I2C_LiquidCrystal_clear(struct LiquidCrystal_I2C *lcd);

void  I2C_LiquidCrystal_home(struct LiquidCrystal_I2C *lcd);

void  I2C_LiquidCrystal_setCursor(struct LiquidCrystal_I2C *lcd, uint8_t col, uint8_t row);

void  I2C_LiquidCrystal_noDisplay(struct LiquidCrystal_I2C *lcd);

void  I2C_LiquidCrystal_display(struct LiquidCrystal_I2C *lcd);

void  I2C_LiquidCrystal_noCursor(struct LiquidCrystal_I2C *lcd);

void  I2C_LiquidCrystal_cursor(struct LiquidCrystal_I2C *lcd);

void  I2C_LiquidCrystal_noBlink(struct LiquidCrystal_I2C *lcd);

void  I2C_LiquidCrystal_blink(struct LiquidCrystal_I2C *lcd);

void  I2C_LiquidCrystal_scrollDisplayLeft(struct LiquidCrystal_I2C *lcd);

void  I2C_LiquidCrystal_scrollDisplayRight(struct LiquidCrystal_I2C *lcd);

void I2C_LiquidCrystal_rightToRight(struct LiquidCrystal_I2C *lcd);

void I2C_LiquidCrystal_rightToLeft(struct LiquidCrystal_I2C *lcd);

void I2C_LiquidCrystal_autoscroll(struct LiquidCrystal_I2C *lcd);

void I2C_LiquidCrystal_noAutoscroll(struct LiquidCrystal_I2C *lcd);

void I2C_LiquidCrystal_createChar(struct LiquidCrystal_I2C *lcd, uint8_t location, uint8_t charmap[]);

void I2C_LiquidCrystal_noBacklight(struct LiquidCrystal_I2C *lcd);

void I2C_LiquidCrystal_backlight(struct LiquidCrystal_I2C *lcd);

void I2C_LiquidCrystal_command(struct LiquidCrystal_I2C *lcd, uint8_t value);


#endif /* I2C_LCD_I2C_LCD_H_ */
