/*
 * ADC.c
 *
 *  Created on: 24/10/2014
 *      Author: marko
 *
 *      ADC measurement routine
 */

#include "ADC.h"
#include "FUNtendo.h"

// Initialise ADC
void init_ADC(void)
{
	// Set LCD LED and POWER LOW LED as outputs
	BAT_LOW_LED(OFF); //Make sure it is off before changing direction
	BAT_LOW_LED_DIR(OUT); //Set BATTERY LED I/Os as outputs.

	// Settings for ADC
	SET_INTERNAL_REFERENCE_VOLTAGE;
	SET_ADC_PIN;
	LEFT_ALIGN_ADC_RESULT;
	ENABLE_ADC;
	AUTO_TRIGGER_ADC;
	TRIGGER_ADC_ON_COUNTER_0_COMPARE_MATCH;
	ADC_INTERRUPT_ENABLE;
	return;
}

// What to do after taking measurement
ISR(ADC_vect)
{
	byte battery_voltage;
	// Read High register - only 8 bit precision
	// 0x00 = 0V 0xFF = 2.56V
	battery_voltage = ADCH;
	if ( battery_voltage < BATTERY_LOW_THRESHOLD ) BAT_LOW_LED(ON);
	else BAT_LOW_LED(OFF);
}
