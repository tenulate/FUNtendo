/*
 * LCD.c
 *
 *  Created on: 19/10/2014
 *      Author: marko
 *
 *      Take care of LCD pin set up and displaying pixels
 */


#include "LCD.h"
#include "FUNtendo.h"
#include "SPI.h"
#include <util/delay.h>

byte LCDPR = LCDPR_INITIAL;	// LCD Power Register: POWER SETTINGS on LCD
byte LCDAC = LCDAC_INITIAL; // LCD Advanced Control register: page/col wrap settings + temp comp.

// *********************************************************
// Higher level drawing functions
// *********************************************************

byte frame_buffer[MAX_PAGES][MAX_COLUMNS] = {{0}};	// store screen image

// Clear all display info in buffer
void clear_frame_buffer(void)
{
	int page_count, col_count;
	for (page_count=0; page_count < MAX_PAGES; page_count++)
	{
		for (col_count=0; col_count < MAX_COLUMNS; col_count++)
		{
			frame_buffer[page_count][col_count] = CLEAR;
		}
	}
	return;
}

// Write what's in memory to screen
void display_frame_buffer(void)
{
	// Go through each page and Column of frame buffer, send data to LCD
	int page_count, col_count;
	for (page_count=0; page_count < MAX_PAGES; page_count++)
	{
		set_page(page_count);
		for (col_count=0; col_count < MAX_COLUMNS; col_count++)
		{
			set_column(col_count);
			LCD_tx_display(frame_buffer[page_count][col_count]);
		}
	}
	return;
}

// Set a single pixel on screen
void set_pixel(byte row, byte col, byte state)
{
	// row = 0 to MAX_ROWS-1
	// col = 0 to MAX_COLUMNS-1
	// state = CLEAR/FILL

	byte page, pixel_mask, current_byte;
	// sanitise input
	if (row >= MAX_ROWS) 	row = MAX_ROWS-1;
	if (col >= MAX_COLUMNS)	col = MAX_COLUMNS-1;

	page = ROW_TO_PAGE(row);
	pixel_mask = _BV(row % ROWS_PER_PAGE);
	current_byte = frame_buffer[page][col];
	frame_buffer[page][col] = ( (pixel_mask & state) | (current_byte & ~pixel_mask) );
	// send this data to LCD
	set_page(page);
	set_column(col);
	LCD_tx_display(frame_buffer[page][col]);
	return;
}

// Get value of single pixel on screen
byte get_pixel(byte row, byte col)
{
	// return CLEAR or FILL
	// sanitise inputs
	byte page, pixel_mask, pixel_byte;
	if (row >= MAX_ROWS) 	row = MAX_ROWS-1;
	if (col >= MAX_COLUMNS)	col = MAX_COLUMNS-1;
	page = ROW_TO_PAGE(row);
	pixel_mask = _BV(row % ROWS_PER_PAGE);
	pixel_byte = frame_buffer[page][col];
	if (pixel_byte & pixel_mask )	return FILL;
	else return CLEAR;
}

// Clear all data on screen
void clear_screen(void)
{
	int page_count; int col_count;
	// Go through every page and column
	for(page_count = 0; page_count < MAX_PAGES; page_count++)
	{
		set_page(page_count);
		for(col_count = 0; col_count < MAX_COLUMNS; col_count++)
		{
			set_column(col_count);
			LCD_tx_display(CLEAR);	// Clear current column
		}
	}
	return;
}

// Make a checker board pattern
void checker_board(void)
{
	int page_count; int col_count; byte pix;
	pix = FILL;
	// Go through every page and column
	for(page_count = 0; page_count < MAX_PAGES; page_count++)
	{
		set_page(page_count);
		if(page_count % 8 == 0)
		{
			pix = ~pix;
		}
		for(col_count = 0; col_count < MAX_COLUMNS; col_count++)
		{
			set_column(col_count);
			if(col_count % 8 == 0)
			{
				pix = ~pix;
			}
			LCD_tx_display(pix);	// Clear current column
		}
	}
	return;
}

// *********************************************************
// Lower level LCD drawing functions
// *********************************************************

// Send either COMMAND or DISPLAY data to LCD controller
void LCD_tx(byte data_type, byte tx_byte)
{
	LCD_CHIP(ENABLE);
	LCD_DATA(data_type);
	SPI_tx(tx_byte);
	LCD_CHIP(DISABLE);
	return;
}

// Send COMMAND data to LCD controller
void LCD_tx_command(byte tx_byte)
{
	LCD_tx(COMMAND_SIGNAL, tx_byte);
}

// Send DISPLAY data to LCD controller
void LCD_tx_display(byte tx_byte)
{
	LCD_tx(DISPLAY_SIGNAL, tx_byte);
}

// Initialises the screen, sets the appropriate pins on microproc
void init_LCD(void)
{
	// set up PWM for back light
	set_PWM(ENABLE);
	set_brightness(INITIAL_BRIGHTNESS);
	// Disable LCD CS and set pin as output
	LCD_CHIP(DISABLE); LCD_CHIP_DIR(OUT);
	// Turn off LCD_CD and set pin as output
	LCD_DATA(LOW); LCD_DATA_DIR(OUT);
	// Set LCD chip pin as output
	LCD_RESET_DIR(OUT);
	// Reset LCD chip
	LCD_RESET(ENABLE); LCD_RESET(DISABLE);

	// Code given in DOGS data sheet to initialise LCD screen
	set_startline(INITIAL_START_LINE);	// Display start line 0
	set_seg_direction(MIRROR); 			// SEG reverse
	set_COM_direction(NORMAL);			// Normal COM0~COM63
	set_black_screen(OFF);
	set_inverted_display(OFF);
	_delay_ms(LCD_INITIAL_WAIT_TIME);
	set_bias_ratio(BIAS_1_ON_9);
	set_LCD_power_booster(ON);
	set_LCD_power_regulator(ON);
	set_LCD_power_follower(ON);
	set_LCD_resistor_ratio(LCD_INITIAL_RESISTOR_RATIO);
	set_contrast(INITIAL_CONTRAST);
	set_temperature_compensation(ON);
	set_page_wrap(OFF);
	set_column_wrap(OFF);
	enable_LCD(ON);
	return;
}

// Select which page to send data to
void set_page(byte page)
{
	byte tx_byte;
	// Make sure page is < MAX_PAGE
	if (page >= MAX_PAGES) {
		page = MAX_PAGES - 1;
	}
	tx_byte = (SET_PAGE | (page & MASK_PAGE_VAL));	// First 4 bits = "page" command for LCD. Last 4 = which page
	LCD_tx_command(tx_byte);
	return;
}

// Select which column to send data to
void set_column(byte col)
{
	byte tx_MSB;
	byte tx_LSB;
	// Make sure col < MAX_COLUMN
	if (col >= MAX_COLUMNS)
	{
		col = MAX_COLUMNS - 1;
	}
	tx_LSB = (SET_COL_LSB | (GET_LSB(col) & MASK_COL_LSB_VAL) ); // Set LSB of column
	tx_MSB = (SET_COL_MSB | (GET_MSB(col) & MASK_COL_MSB_VAL) ); // Set MSB of column
	LCD_tx_command(tx_LSB);
	LCD_tx_command(tx_MSB);
	return;
}

// **********************************************************
// PWM Functionality
// **********************************************************

// Change a bit vector to ON/OFF. Eg. 0010 0000 -> 1111 1111
// Used to set registers from single bit value
byte expand_bit(byte bit_vector)
{
	if (bit_vector)	return ON;
	else return OFF;
}

// set up and turn on/off PWM mode
void set_PWM(byte state)
{
	// state = ENABLE/DISABLE
	if(state == ENABLE)
	{
		set_PWM_force_output_compare(OFF);
		set_PWM_compare_output_mode(CLEAR_OC2_ON_MATCH);
		set_PWM_wave_gen_mode(PWM_FAST);
		set_PWM_clock_scaling(CLK_SCALE_1);
		LCD_LED_DIR(OUT);
	}
	else
	{
		set_PWM_clock_scaling(STOP_PWM_CLOCK);
		set_PWM_compare_output_mode(OC2_DISCONNECT);
	}
	return;
}
// Set Wave gen mode
void set_PWM_wave_gen_mode(byte mode)
{
	// mode = NORMAL_MODE, PWM_PHASE_CORRECT, CTC_MODE, PWM_FAST
	byte wgm20, wgm21;
	// get register values from mode
	wgm20 = expand_bit(GET(mode, MASK_WGM20_VAL));
	wgm21 = expand_bit(GET(mode, MASK_WGM21_VAL));
	// set appropriate register
	SET_WGM20(wgm20);
	SET_WGM21(wgm21);
	return;
}

// sets clock scaling for the TCCR2 register
void set_PWM_clock_scaling(byte clock_scale)
{
	// clock_scale = NO_CLOK_SRC, CLK_SCALE_x where x = 1, 8, 32, 64, 128, 256, 1024
	byte cs20, cs21, cs22;
	// get appropriate bits from clock_scale value
	cs20 = expand_bit(GET(clock_scale, MASK_CS20_VAL));
	cs21 = expand_bit(GET(clock_scale, MASK_CS21_VAL));
	cs22 = expand_bit(GET(clock_scale, MASK_CS22_VAL));
	// set appropriate registers
	SET_CS20(cs20);
	SET_CS21(cs21);
	SET_CS22(cs22);
	return;
}

// Set/Unset force on output compare
void set_PWM_force_output_compare(byte foc)
{
	// foc = ON/OFF
	SET_FOC2(expand_bit(foc));
	return;
}

// Set pin out (OC2) behaviour when timer = compare register value (TCCR2 = OCR2)
void set_PWM_compare_output_mode(byte mode)
{
	// mode = OC2_DISCONNECT, CLEAR_OC2_ON_MATCH, SET_OC2_ON_MATCH
	byte com20, com21;
	// get appropriate bits from mode
	com20 = expand_bit(GET(mode, MASK_COM20_VAL));
	com21 = expand_bit(GET(mode, MASK_COM21_VAL));
	// set appropriate registers
	SET_COM20(com20);
	SET_COM21(com21);
	return;
}

// change brightness of screen by changing fraction of time which PWM pin (OC2) turns on
void set_brightness(byte brightness)
{
	// brightness ranges from 0x00 (NO LCD LED) to 0xFF (FULL LCD LED)
	// sanitise input
	SET_OCR2(brightness);
	return;
}

// **********************************************************
// Boiler plate stuff for initialising LCD
// **********************************************************

// Enable / Disable (sleep) LCD
void enable_LCD(byte state)
{
	// state = ENABLE / (SLEEP or DISABLE)
	byte tx_byte;
	tx_byte = (SET_ENABLE_LCD | (state & MASK_ENABLE_LCD_VAL));
	LCD_tx_command(tx_byte);
	return;
}
// Set the BIAS RATIO of LCD to 1/9 or 1/7
void set_bias_ratio(byte ratio)
{
	// Ratio = BIAS_1_ON_9 / BIAS_1_ON_7
	byte tx_byte;
	tx_byte = (SET_BIAS_RATIO | (ratio & MASK_BIAS_VAL) );
	LCD_tx_command(tx_byte);
	return;
}

// Activates all pixels but does not change LCD display memory
void set_black_screen(byte state)
{
	// set = ON / OFF
	byte tx_byte;
	tx_byte = (SET_BLACK_SCREEN | (state & MASK_BLACK_SCREEN_VAL));
	LCD_tx_command(tx_byte);
	return;
}

// Set communication direction for LCD screen
void set_COM_direction(byte direction)
{
	// direction = NORMAL/MIRROR
	byte tx_byte;
	tx_byte = (SET_COM_DIR | (direction & MASK_COM_VAL));
	LCD_tx_command(tx_byte);
	return;
}

// Set the contrast of LCD
void set_contrast(byte contrast)
{
	// contrast = 0 - 63: Mask takes care of this data range
	LCD_tx_command(SET_CONTRAST);	// First BYTE tells LCD setting contrast
	contrast &= MASK_CONTRAST_VAL;	// Get value bits
	LCD_tx_command(contrast);
	return;
}

// Make the LCD display the inverse of what's stored in LCD's SRAM
void set_inverted_display(byte state)
{
	// SET = ON / OFF
	byte tx_byte;
	tx_byte = (SET_INVERTED | (state & MASK_INVERTED_VAL));
	LCD_tx_command(tx_byte);
	return;
}

// Turn power booster on LCD ON/OFF
void set_LCD_power_booster(byte state)
{
	byte tx_byte;
	SET(LCDPR, MASK_BOOSTER_VAL, state);
	tx_byte = (SET_POWER_CONTROL | LCDPR);
	LCD_tx_command(tx_byte);
	return;
}

// Turn power follower on LCD ON/OFF
void set_LCD_power_follower(byte state)
{
	byte tx_byte;
	SET(LCDPR, MASK_FOLLOWER_VAL, state);
	tx_byte = (SET_POWER_CONTROL | LCDPR);
	LCD_tx_command(tx_byte);
	return;
}

// Turn power regulator on LCD ON/OFF
void set_LCD_power_regulator(byte state)
{
	byte tx_byte;
	SET(LCDPR, MASK_REGULATOR_VAL, state);
	tx_byte = (SET_POWER_CONTROL | LCDPR);
	LCD_tx_command(tx_byte);
	return;
}

// Set resistor ratio of LCD (an internal setting - not sure what resistors
void set_LCD_resistor_ratio(byte ratio)
{
	// ratio = 0, 1, 2, ..., 7 -> Mask will take care of this
	byte tx_byte;
	tx_byte = (SET_LCD_RESISTOR_RATIO | (ratio & MASK_LCD_RESISTOR_RATIO_VAL));
	LCD_tx_command(tx_byte);
	return;
}

// Physical orientation of screen (NORMAL for data pins at bottom, MIRROR data pins top)
void set_seg_direction(byte direction)
{
	// direction = NORMAL/MIRROR
	byte tx_byte;
	tx_byte = (SET_SEG_DIR | (direction & MASK_SEG_VAL));
	LCD_tx_command(tx_byte);
	return;
}

// Where the cursor starts out (?)
void set_startline(byte start_line)
{
	// Sanitise input
	byte tx_byte;
	if (start_line >= MAX_SCROLL_LINES)
	{
		start_line = MAX_SCROLL_LINES-1;
	}
	tx_byte = (SET_START_LINE | start_line);
	LCD_tx_command(tx_byte);
	return;
}

// Sets whether Column is wrapped or not
void set_column_wrap(byte state)
{
	// state = ON/OFF
	byte tx_byte;
	SET(LCDAC, MASK_COLUMN_WRAP_VAL, state);
	LCD_tx_command(INIT_ADVANCED_CONTROL);
	tx_byte = (SET_ADVANCED_CONTROL | LCDAC);
	LCD_tx_command(tx_byte);
	return;
}

void set_page_wrap(byte state)
{
	// state = ON/OFF
	byte tx_byte;
	SET(LCDAC, MASK_PAGE_WRAP_VAL, state);
	LCD_tx_command(INIT_ADVANCED_CONTROL);
	tx_byte = (SET_ADVANCED_CONTROL | LCDAC);
	LCD_tx_command(tx_byte);
	return;
}

void set_temperature_compensation(byte state)
{
	byte tx_byte;
	SET(LCDAC, MASK_TEMPERATURE_COMPENSATION_VAL, state);
	LCD_tx_command(INIT_ADVANCED_CONTROL);
	tx_byte = (SET_ADVANCED_CONTROL | LCDAC);
	LCD_tx_command(tx_byte);
	return;
}


/*
// Bit banging version of transmitting data
byte LCD_tx(byte tx_byte, byte data_type)
{
	// data_type = DISPLAY or COMMAND
	byte tx_processed;
	byte tx_mask = TX_MASK;
	LCD_CHIP(ENABLE);
	LCD_DATA(data_type);
	while (tx_mask != TX_FINISH)
	{
		tx_processed = tx_byte & tx_mask;
		SCK_SET(HIGH);
		if(tx_processed)
		{
			MOSI_SET(HIGH);
		}
		else
		{
			MOSI_SET(LOW);
		}
		SCK_SET(LOW);
		tx_mask>>=1;	// Right shift mask 1 (1000 0000 -> 0100 000)
	}
	SCK_SET(HIGH);
	LCD_CHIP(DISABLE);
	return(TRUE);
}
*/

