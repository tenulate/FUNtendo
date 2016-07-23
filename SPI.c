/*
 * SPI.c
 *
 *  Created on: 20/10/2014
 *      Author: marko
 *
 *      Take care of all SPI set up and interfacing
 */

#include "FUNtendo.h"
#include "SPI.h"

// Initialise SPI module
void init_SPI(void)
{
	// Set SPI pins
	SCK_SET(LOW);	SCK_DIR(OUT);
	MISO_SET(LOW);	MISO_DIR(IN);
	MOSI_SET(LOW);	MOSI_DIR(OUT);
	SPI_SS(HIGH);	SPI_SS_DIR(OUT);	// Make sure Slave select is output to act as master
	// Enable SPI as master
	SPI(ENABLE);	SPI_MODE(MASTER);
	set_SCK_freq(CLK_DIV_BY_4);
	return;
}

// Transmit data over SPI interface
void SPI_tx(byte data)
{
	// Load data into register
	SPDR = data;
	// Wait for transmission to complete
	while( (~SPI_FINISHED) );
	return;
}

// Set SPI CLOCK frequency as fraction of CPU CLK
void set_SCK_freq(byte freq) {
	// freq = CLK_DIV_BY_n where n = 2, 4, 8, 16, 32, 64, 128
	freq &= MASK_CLK_VAL;	// sanitise input
	byte spr0, spr1, spi2x;
	// get register values from freq
	spr0 = freq & MASK_SPR0_VAL;
	spr1 = freq & MASK_SPR1_VAL;
	spi2x = freq & MASK_SPI2X_VAL;
	// set appropriate registers
	SET_SPR0(spr0);
	SET_SPR1(spr1);
	SET_SPI2X(spi2x);
	return;
}
