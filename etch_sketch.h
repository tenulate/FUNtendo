/*
 * etch_sketch.h
 *
 *  Created on: 22/10/2014
 *      Author: marko
 */

#ifndef ETCH_SKETCH_H_
#define ETCH_SKETCH_H_

#include "FUNtendo.h"

#define ROW_START 32
#define COL_START 51
#define BUTTON_DELAY 255
#define BACKLIGHT_BUTTON_DELAY 750
#define BRIGHTNESS_INCREMENT 0x19	// 10% of 255 for 10% change

// function prototypes
byte play_etch_sketch(void);

#endif /* ETCH_SKETCH_H_ */
