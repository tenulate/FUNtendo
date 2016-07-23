#ifndef FUNTENDO_H_
#define FUNTENDO_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define byte unsigned char
#define byte2 uint16_t

/*ON OFF*/
#define ON 0xFF
#define OFF 0x00
#define HIGH 0xFF
#define LOW 0x00
#define IN 0x00
#define OUT 0xFF
#define ENABLE 0xFF
#define DISABLE 0x00
#define ALL 0xFF
#define TRUE 1
#define FALSE 0
#define FORWARD 0x00
#define BACK 0xFF
#define INFINITE_LOOP 1
#define EXIT_GOOD 0
#define EXIT_ERROR 1

/*SET and GET MACRO*/
#define SET(PORT,MASK,VALUE) PORT = ((MASK & VALUE) | (PORT & ~MASK))
#define GET(PORT,MASK) PORT & MASK

#endif /* FUNTENDO_H_ */
