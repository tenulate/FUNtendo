/*
 * buttons.c
 *
 *  Created on: 19/10/2014
 *      Author: marko
 *
 *      Initialise buttons and set pull ups
 */

#include "buttons.h"
#include "FUNtendo.h"
#include <avr/interrupt.h>

// Flags for which buttons have been pushed
byte up_button = NOT_PRESSED;
byte down_button = NOT_PRESSED;
byte left_button = NOT_PRESSED;
byte right_button = NOT_PRESSED;
byte a_button = NOT_PRESSED;
byte b_button = NOT_PRESSED;

// Interrupt routine to set button flags
// External interrupt to happen on rising edge from NAND gate
ISR(INT1_vect)
{
	if (UP_BUTTON)		up_button = PRESSED;
	if (LEFT_BUTTON) 	left_button = PRESSED;
	if (RIGHT_BUTTON)	right_button = PRESSED;
	if (DOWN_BUTTON)	down_button = PRESSED;
	if (A_BUTTON)		a_button = PRESSED;
	if (B_BUTTON)		b_button = PRESSED;
}

// Interrupt routine from TIMER0 - use to set delay between registering button pushes
ISR(TIMER0_COMP_vect)
{
	static byte timer0_cycles = 0;
	timer0_cycles++;
	// Pole the push button states if we've waited long enough
	if (timer0_cycles >= TIME_BETWEEN_BUTTON_PUSHES)
	{
		timer0_cycles = 0;
		if (UP_BUTTON) 		up_button = PRESSED;
		if (LEFT_BUTTON) 	left_button = PRESSED;
		if (RIGHT_BUTTON)	right_button = PRESSED;
		if (DOWN_BUTTON)	down_button = PRESSED;
		if (A_BUTTON)		a_button = PRESSED;
		if (B_BUTTON)		b_button = PRESSED;
	}
}

// Initialise all buttons as inputs, set pull up resistors
void init_buttons(void)
{
	// Set all buttons as inputs
	D_PAD_BUTTONS_DIR(IN);
	AB_BUTTONS_DIR(IN);

	// enable pull up resistors
	GLOBAL_PULL_UP_RESISTORS(ENABLE);
	D_PAD_BUTTONS_PULL_UP(ENABLE);
	AB_BUTTONS_PULL_UP(ENABLE);

	// enable interrupts
	sei();
	ACTIVATE_INT1_RISING_EDGE;
	ENABLE_INT1;
	// timer interrupts
	set_timer0_clock_scaling(INT_CLK_DIV_BY_64);
	ENABLE_TIMER_INTERRUPT;
	return;
}

// sets all buttons to state NOT_PRESSED
void clear_button_flags(void)
{
	up_button = NOT_PRESSED;
	down_button = NOT_PRESSED;
	left_button = NOT_PRESSED;
	right_button = NOT_PRESSED;
	a_button = NOT_PRESSED;
	b_button = NOT_PRESSED;
	return;
}

// Set timer0 counter to update on every Nth clock cycle
void set_timer0_clock_scaling(byte scale)
{
	switch(scale)
	{
	case INT_CLK_DIV_BY_1024:
		SET_CS00(HIGH);
		SET_CS01(LOW);
		SET_CS02(HIGH);
		break;
	case INT_CLK_DIV_BY_256:
		SET_CS00(LOW);
		SET_CS01(LOW);
		SET_CS02(HIGH);
		break;
	case INT_CLK_DIV_BY_64:
		SET_CS00(HIGH);
		SET_CS01(HIGH);
		SET_CS02(LOW);
		break;
	case INT_CLK_DIV_BY_8:
		SET_CS00(LOW);
		SET_CS01(HIGH);
		SET_CS02(LOW);
		break;
	default:
		// no prescaling
		SET_CS00(HIGH);
		SET_CS01(LOW);
		SET_CS02(LOW);
		break;
	}
	return;
}

