/*
 * LCD.h
 *
 *  Created on: 19/10/2014
 *      Author: marko
 */

#ifndef LCD_H_
#define LCD_H_

#include "FUNtendo.h"	// need this for "BYTE" definition

// LCD Power Register: Remember power settings of LCD
extern byte LCDPR;
#define LCDPR_INITIAL 0x00
// LCD Advanced Control register: page/col wrap and temp compensation settings
extern byte LCDAC;
#define LCDAC_INITIAL 0x00

// LCD data transmission variables
#define COMMAND_SIGNAL 0x00	// LCD CD pin when sending command
#define DISPLAY_SIGNAL 0xFF	// LCD CD pin when sending display data
#define CLEAR 0x00			// LCD data when clearing display
#define FILL 0xFF			// LCD data when filling in display
#define NORMAL 0x00			// LCD SEG and COMM directions, and (don't) display INVERSE pixels
#define MIRROR 0xFF			// LCD SEG and COMM directions
#define INVERTED 0xFF		// LCD display (NORMAL=display LCD SRAM / INVERTED=display inverse of SRAM)
#define BIAS_1_ON_9	0x00	// LCD BIAS RATIO 1/9
#define BIAS_1_ON_7	0xFF	// LCD BIAS RATIO 1/7
#define SLEEP 0x00			// set display to sleep mode

// Parameters for LCD display
#define MAX_PAGES 8			// Max number of pages of LCD
#define MAX_COLUMNS 102		// Max number of columns on LCD
#define MAX_ROWS 64			// Max numbe of rows on LCD
#define MAX_SCROLL_LINES 64	// Max number of scroll lines on LCD
// Useful parameters when displaying
#define SCREEN_WIDTH MAX_COLUMNS
#define SCREEN_HEIGHT MAX_ROWS
#define SCREEN_TOP 0x00
#define SCREEN_LEFT 0x00
#define SCREEN_BOTTOM MAX_ROWS
#define SCREEN_RIGHT MAX_COLUMNS

// LCD Start Up parameters
#define INITIAL_START_LINE 0
#define LCD_INITIAL_WAIT_TIME 120		// wait time in ms for LCD initialisation
#define LCD_INITIAL_RESISTOR_RATIO 7
#define INITIAL_CONTRAST 16				// Contrast = 0-63
#define INITIAL_BRIGHTNESS 0x88

// COMMANDS for LCD chip
#define SET_PAGE 0xB0				// Set page address command to LCD controller
#define SET_COL_LSB 0x00			// Least Significant Bits for column select
#define SET_COL_MSB 0x10			// Most Significant Bits for column select
#define SET_COM_DIR 0xC0			// Set Communication direction
#define SET_BLACK_SCREEN 0xA4		// Set all pixels on - but doesn't save this to SRAM of LCD
#define SET_INVERTED 0xA6			// Display pixels
#define SET_START_LINE 0x40			// Set starting line number
#define SET_POWER_CONTROL 0x28		// Set BOOSTER, REGULATOR, FOLLOWER on LCD
#define SET_LCD_RESISTOR_RATIO 0x20 // Configure internal Resistor ratio of LCD
#define SET_CONTRAST 0x81			// Command to initialise setting contrast of LCD
#define SET_SEG_DIR 0xA0			// Set the direction/orientation of screen
#define SET_BIAS_RATIO 0xA2			// Set LCD Bias ratio
#define INIT_ADVANCED_CONTROL 0xFA	// First command to enter Advanced control settings
#define SET_ADVANCED_CONTROL 0x10	// Advanced control signal
#define SET_ENABLE_LCD 0xAE			// Enable / Disable (sleep) display

// MASKS for the argument bits of different LCD commands
#define MASK_PAGE_VAL 0x0F		// Last 4 bits of SET_PAGE command represent page value
#define MASK_COL_LSB_VAL 0x0F
#define MASK_COL_MSB_VAL 0x0F
#define MASK_COM_VAL 0x08
#define MASK_BLACK_SCREEN_VAL 0x01
#define MASK_INVERTED_VAL 0x01
#define MASK_START_LINE_VAL 0x3F	// Last 6 bits
#define MASK_BOOSTER_VAL 0x01
#define MASK_REGULATOR_VAL 0x02
#define MASK_FOLLOWER_VAL 0x04
#define MASK_LCD_RESISTOR_RATIO_VAL 0x07
#define MASK_CONTRAST_VAL 0x3F
#define MASK_SEG_VAL 0x01
#define MASK_BIAS_VAL 0x01
#define MASK_ADVANCED_CONTROL_VAL 0x83
#define MASK_COLUMN_WRAP_VAL 0x02
#define MASK_PAGE_WRAP_VAL 0x01
#define MASK_TEMPERATURE_COMPENSATION_VAL 0x80
#define MASK_ENABLE_LCD_VAL 0x01

// PIN MACROS
// State and Direction of LCD LED
#define LCD_LED(STATE) SET(PORTD, _BV(PD7), STATE)	// STATE = ON/OFF
#define LCD_LED_DIR(DIR) SET(DDRD, _BV(PD7), DIR)	// DIR = IN/OUT
// State and Direction of LCD Chip Select
#define LCD_CHIP(STATE) SET(PORTB, _BV(PB1), ~STATE) 	// STATE = ENABLE/DISABLE (active low)
#define LCD_CHIP_DIR(DIR) SET(DDRB, _BV(PB1), DIR)		// DIR = IN/OUT
// What type of data and Direction of LCD Data pin
#define LCD_DATA(DATA_TYPE) SET(PORTB, _BV(PB3), DATA_TYPE)	// DATA_TYPE = COMMAND_SIGNAL/DISPLAY
#define LCD_DATA_DIR(DIR) SET(DDRB, _BV(PB3), DIR)			// DIR = IN/OUT
// State and direction of LCD Reset pin
#define LCD_RESET(STATE) SET(PORTB, _BV(PB2), ~STATE) 	// STATE = ENABLE/DISABLE (active low)
#define LCD_RESET_DIR(DIR) SET(DDRB, _BV(PB2), DIR)		// DIR = IN/OUT

// Sending Data MACROs
#define BYTE_LSB 0x0F	// Lower nibble of a Byte ( xxxx ABCD -> 0000 ABCD)
#define BYTE_MSB 0xF0	// Upper nibble of a Byte (EFGH xxxx -> EFGH 0000)
#define GET_MSB(DATA) DATA>>4			// Right shift 4 bits: JKLM xxxx >> 4 = 0000 JKLM
#define GET_LSB(DATA) DATA & BYTE_LSB	// Mask to get Least Significant Bits

// Pixel/Row Conversion MACROS
#define ROWS_PER_PAGE 8
#define LOG2_ROWS_PER_PAGE 3	// There are 2^3 rows in a page
#define ROW_TO_PAGE(ROW) (ROW>>LOG2_ROWS_PER_PAGE)

// prototype functions
void init_LCD(void);
void LCD_tx(byte data_type, byte tx_byte);
void LCD_tx_command(byte tx_byte);
void LCD_tx_display(byte tx_byte);
void set_page(byte page);
void set_column(byte col);
void clear_screen(void);
void checker_board(void);
void set_bias_ratio(byte ratio);
void set_black_screen(byte);
void set_COM_direction(byte direction);
void set_contrast(byte contrast);
void set_inverted_display(byte state);
void set_LCD_power_booster(byte state);
void set_LCD_power_regulator(byte state);
void set_LCD_power_follower(byte state);
void set_seg_direction(byte direction);
void set_startline(byte start_line);
void set_LCD_resistor_ratio(byte ratio);
void set_column_wrap(byte state);
void set_page_wrap(byte state);
void set_temperature_compensation(byte state);
void enable_LCD(byte state);

// Higher level utilities
extern byte frame_buffer[MAX_PAGES][MAX_COLUMNS];	// store screen image
void set_pixel(byte row, byte col, byte state);
byte get_pixel(byte row, byte col);
void clear_frame_buffer(void);
void display_frame_buffer(void);

// PWM utilities
void set_PWM_wave_gen_mode(byte mode);
void set_PWM_clock_scaling(byte clock_scale);
void set_PWM_force_output_compare(byte foc);
void set_PWM_compare_output_mode(byte mode);
void set_brightness(byte bright);
void set_PWM(byte state);
byte expand_bit(byte bit_vector);

#define MAX_BRIGHTNESS 0xFF
#define MIN_BRIGHTNESS 0x00

// Wave Generation Modes (TCCR2 WGM bits)
#define NORMAL_MODE 0x00
#define PWM_PHASE_CORRECT 0x01
#define CTC_MODE 0x02
#define PWM_FAST 0x03
#define SET_WGM20(STATE) SET(TCCR2, _BV(WGM20), STATE)
#define SET_WGM21(STATE) SET(TCCR2, _BV(WGM21), STATE)
#define MASK_WGM20_VAL 0x01
#define MASK_WGM21_VAL 0x02
// Compare OUTPUT modes for Fast PWM and Phase correct PWM (TCCR2 COM2 bits)
#define OC2_DISCONNECT 0x00
#define CLEAR_OC2_ON_MATCH 0x02
#define SET_OC2_ON_MATCH 0x03
#define SET_COM20(STATE) SET(TCCR2, _BV(COM20), STATE)
#define SET_COM21(STATE) SET(TCCR2, _BV(COM21), STATE)
#define MASK_COM20_VAL 0x01
#define MASK_COM21_VAL 0x02
// Clock select for CS2 (TCCR2 CS2 bits)
#define STOP_PWM_CLOCK 0x00
#define CLK_SCALE_1 0x01
#define CLK_SCALE_8 0x02
#define CLK_SCALE_32 0x03
#define CLK_SCALE_64 0x04
#define CLK_SCALE_128 0x05
#define CLK_SCALE_256 0x06
#define CLK_SCALE_1024 0X07
#define SET_CS20(STATE) SET(TCCR2, _BV(CS20), STATE)
#define SET_CS21(STATE) SET(TCCR2, _BV(CS21), STATE)
#define SET_CS22(STATE) SET(TCCR2, _BV(CS22), STATE)
#define MASK_CS20_VAL 0x01
#define MASK_CS21_VAL 0x02
#define MASK_CS22_VAL 0x04
// Set output compare value
#define SET_OCR2(CMP_VAL) SET(OCR2, ALL, CMP_VAL)
// Force Output compare (TCCR2 FOC2 bit)
#define SET_FOC2(STATE) SET(TCCR2, _BV(FOC2), STATE)

#endif /* LCD_H_ */
