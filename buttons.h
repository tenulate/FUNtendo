#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "FUNtendo.h"

// Read state of each button. All buttons are active low
#define UP_BUTTON ~GET(PINA, _BV(PA4))
#define LEFT_BUTTON ~GET(PINA, _BV(PA5))
#define DOWN_BUTTON ~GET(PINA, _BV(PA6))
#define RIGHT_BUTTON ~GET(PINA, _BV(PA7))
#define A_BUTTON ~GET(PINC, _BV(PC7))
#define B_BUTTON ~GET(PINC, _BV(PC6))

// Where on PORTA are D-PAD buttons connected
#define D_PAD_PINS ( _BV(PA4) | _BV(PA5) | _BV(PA6) | _BV(PA7) )
// Where on PORTC are buttons A and B connected
#define AB_PINS ( _BV(PC6) | _BV(PC7) )

// Button direction MACROs for individual buttons
#define UP_BUTTON_DIR(DIR) SET(DDRA, _BV(PA4), DIR)
#define LEFT_BUTTON_DIR(DIR) SET(DDRA, _BV(PA5), DIR)
#define DOWN_BUTTON_DIR(DIR) SET(DDRA, _BV(PA6), DIR)
#define RIGHT_BUTTON_DIR(DIR) SET(DDRA, _BV(PA7), DIR)
#define A_BUTTON_DIR(DIR) SET(DDRC, _BV(PC7), DIR)
#define B_BUTTON_DIR(DIR) SET(DDRC, _BV(PC6), DIR)
// Button direction MACROs for multiple buttons
// Change all DPAD buttons to input/output
#define D_PAD_BUTTONS_DIR(DIR) SET(DDRA, D_PAD_PINS, DIR)	// DIR = IN/OUT
// Change A and B buttons to input/output
#define AB_BUTTONS_DIR(DIR) SET(DDRC, AB_PINS, DIR) 		// DIR = IN/OUT

// Pull up resistor MACROs for individual buttons
#define UP_BUTTON_PULL_UP(STATE) SET(PORTA, _BV(PA4), STATE)
#define LEFT_BUTTON_PULL_UP(STATE) SET(PORTA, _BV(PA5), STATE)
#define DOWN_BUTTON_PULL_UP(STATE) SET(PORTA, _BV(PA6), STATE)
#define RIGHT_BUTTON_PULL_UP(STATE) SET(PORTA, _BV(7), STATE)
#define A_BUTTON_PULL_UP(STATE) SET(PORTC, _BV(PC7), STATE)
#define B_BUTTON_PULL_UP(STATE) SET(PORTC, _BV(PC6), STATE)
// Pull up resistor MACROs for multiple buttons
#define GLOBAL_PULL_UP_RESISTORS(STATE) SET(SFIOR,_BV(PUD), ~STATE);	// STATE = ENABLE/DISABLE (active low)
#define D_PAD_BUTTONS_PULL_UP(STATE) SET(PORTA, D_PAD_PINS, STATE)		// STATE = ENABLE/DISABLE
#define AB_BUTTONS_PULL_UP(STATE) SET(PORTC, AB_PINS, STATE)			// STATE = ENABLE/DISABLE

// Function prototypes
void init_buttons(void);
void clear_button_flags(void);
void set_timer0_clock_scaling(byte scale);

// Interrupts
#define SET_INT1_ACTIVE_LOW SET(MCUCR, (_BV(ISC11)|_BV(ISC10)), LOW)
#define ACTIVATE_INT1_FALLING_EDGE SET(MCUCR, _BV(ISC11), HIGH)
#define ACTIVATE_INT1_RISING_EDGE SET(MCUCR, (_BV(ISC11)|_BV(ISC10)), HIGH)
#define ENABLE_INT1 SET(GICR, _BV(INT1), ENABLE)
#define DISABLE_INT1 SET(GICR, _BV(INT1), DISABLE)

// Timer interrupt
#define TIME_BETWEEN_BUTTON_PUSHES 30
#define ENABLE_TIMER_INTERRUPT SET(TIMSK, _BV(OCIE0), HIGH)
#define DISABLE_TIMER_INTERRUPT SET(TIMSK, _BV(OCIE0), LOW)
#define SET_CS00(STATE) SET(TCCR0, _BV(CS00), STATE)
#define SET_CS01(STATE) SET(TCCR0, _BV(CS01), STATE)
#define SET_CS02(STATE) SET(TCCR0, _BV(CS02), STATE)
#define INT_CLK_DIV_BY_1024	0
#define INT_CLK_DIV_BY_256 1
#define INT_CLK_DIV_BY_64 2
#define INT_CLK_DIV_BY_8 3

// Flags that describe which buttons were pressed in the most recent interrupt
extern byte up_button;
extern byte down_button;
extern byte left_button;
extern byte right_button;
extern byte a_button;
extern byte b_button;

#define PRESSED 0xFF
#define NOT_PRESSED 0x00

// **************************************
// PUT THIS IN ANOTHER HEADER
// Device Outputs

// **************************************

#endif /* INPUT_OUTPUT_H_ */
