/*
 * etch_sketch.c
 *
 *  Created on: 22/10/2014
 *      Author: marko
 *
 *      Plays the etch_sketch game
 */

#include "FUNtendo.h"
#include "LCD.h"
#include "buttons.h"
#include "etch_sketch.h"
#include <util/delay.h>

byte play_etch_sketch(void)
{
	byte row, col, brightness;		// store current pixel location
	row = ROW_START;
	col = COL_START;
	brightness = INITIAL_BRIGHTNESS;
	while(INFINITE_LOOP)
	{
		// coordinate (0,0) is TOP LEFT of screen
		// wrap around by doing modulo arithmetic
		// add screen height/width to avoid wrapping from 0x00 to 0xFF
		if (up_button)
		{
			row = (row-1+SCREEN_HEIGHT) % SCREEN_HEIGHT;
			up_button = NOT_PRESSED;
		}
		if (down_button)
		{
			row = (row+1+SCREEN_HEIGHT) % SCREEN_HEIGHT;
			down_button = NOT_PRESSED;
		}
		if (left_button)
		{
			col = (col-1+SCREEN_WIDTH)  % SCREEN_WIDTH;
			left_button = NOT_PRESSED;
		}
		if (right_button)
		{
			col = (col+1+SCREEN_WIDTH)  % SCREEN_WIDTH;
			right_button = NOT_PRESSED;
		}
		if (a_button)
		{
			// increment brightness - turn off if at max brightness
			if (brightness >= MAX_BRIGHTNESS - BRIGHTNESS_INCREMENT)
			{
				set_PWM(DISABLE);
				LCD_LED(OFF);
			}
			else
			{
				set_PWM(ENABLE);
				set_brightness(brightness);
			}
			brightness += BRIGHTNESS_INCREMENT;
			a_button = NOT_PRESSED;
		}
		if (b_button)
		{
			clear_screen();
			clear_frame_buffer();
			b_button = NOT_PRESSED;
		}

		set_pixel(row, col, FILL);
	}
	return EXIT_GOOD;
}
