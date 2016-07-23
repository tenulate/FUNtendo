/*************************************************************************
Title:    game_console Template
Initial Author:   David Jahshan
Extended by : (PUT YOUR NAME HERE) (PUT YOUR STUDENT NUMBER HERE)
Software: AVR-GCC
Hardware: ATMEGA16 @ 8Mhz

DESCRIPTION:
	Main and functions for Game Console basic sample code

*************************************************************************/
#include "FUNtendo.h"
#include "buttons.h"
#include "LCD.h"
#include "etch_sketch.h"
#include "SPI.h"
#include "ADC.h"
#include <util/delay.h>

int main(void)
{
	init_buttons();	// initialise buttons as inputs
	init_SPI();		// initialise SPI pins
	init_LCD();		// initialise LCD screen controller
	init_ADC();
	clear_screen();	// Clears LCD screen
	clear_frame_buffer();

	play_etch_sketch();

	return EXIT_GOOD;
}
