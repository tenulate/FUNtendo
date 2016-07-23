/*
 * SPI.h
 *
 *  Created on: 20/10/2014
 *      Author: marko
 */

#ifndef SPI_H_
#define SPI_H_

#include "FUNtendo.h"	// needed for ENABLE/DISABLE definitions

#define MASTER 0xFF
#define SLAVE 0x00

// SPI pins
#define SCK_SET(STATE) SET(PORTB, _BV(7), STATE)	// STATE = HIGH/LOW
#define SCK_DIR(DIR) SET(DDRB, _BV(7), DIR)			// DIR = IN/OUT
#define MISO_SET(STATE) SET(PORTB, _BV(6), STATE)	// STATE = HIGH/LOW
#define MISO_DIR(DIR) SET(DDRB, _BV(6), DIR)		// DIR = IN/OUT
#define MOSI_SET(STATE) SET(DDRB, _BV(5), STATE)	// STATE = HIGH/LOW
#define MOSI_DIR(DIR) SET(DDRB, _BV(5), DIR)		// DIR = IN/OUT

// Slave select Macros
#define SPI_SS(STATE) SET(PORTB, _BV(4), STATE)		// STATE = HIGH/LOW
#define SPI_SS_DIR(DIR) SET(DDRB, _BV(4), DIR)		// DIR = IN/OUT

// SPI CONTROL REGISTER
#define SPI_MODE(MODE) SET(SPCR, _BV(MSTR), MODE)	// MODE = MASTER/SLAVE
#define SPI(MODE) SET(SPCR, _BV(SPE), MODE)			// MODE = ENABLE/DISABLE
#define SET_SPR0(STATE) SET(SPCR, _BV(SPR0), STATE)	// STATE = HIGH/LOW
#define SET_SPR1(STATE) SET(SPCR, _BV(SPR1), STATE)	// STATE = HIGH/LOW
#define SET_SPI2X(STATE) SET(SPSR, _BV(SPI2X), STATE) // STATE = HIGH/LOW
#define SPI_FINISHED SPSR & _BV(SPIF)	// Check SPIF bit of SPI Status Register. When == 1 then SPI transmission has finished

// Setting SPI clock frequency
#define CLK_DIV_BY_4 0x00
#define CLK_DIV_BY_16 0x01
#define CLK_DIV_BY_64 0x02
#define CLK_DIV_BY_128 0x03
#define CLK_DIV_BY_2 0x04
#define CLK_DIV_BY_8 0x05
#define CLK_DIV_BY_32 0x06
// Masks to extract SPR0 SPR1 and SPI2X from CLK_DIV_BY_n values
#define MASK_CLK_VAL 0x07
#define MASK_SPR0_VAL 0x01
#define MASK_SPR1_VAL 0x02
#define MASK_SPI2X_VAL 0x04

// Bit Banging variables
#define TX_MASK 0x80	// when transmitting mask with 1000 0000 and right shift it
#define TX_FINISH 0x00	// When finished transmitting mask will be 0000 0000

// prototype function
void init_SPI(void);
void SPI_tx(byte data);
void set_SCK_freq(byte freq);

#endif /* SPI_H_ */
