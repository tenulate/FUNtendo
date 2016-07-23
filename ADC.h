/*
 * ADC.h
 *
 *  Created on: 24/10/2014
 *      Author: marko
 *
 *      Analog to Digital Converter Functionality
 */
#include "FUNtendo.h"

#ifndef ADC_H_
#define ADC_H_

void init_ADC(void);

#define BAT_LOW_LED(STATE) SET(PORTA,_BV(PA2),~STATE)
#define BAT_LOW_LED_DIR(DIR) SET(DDRA, _BV(PA2), DIR)

#define SET_INTERNAL_REFERENCE_VOLTAGE SET(ADMUX, (_BV(REFS0)|_BV(REFS1)), HIGH)
#define SET_ADC_PIN SET(ADMUX, (_BV(MUX0)|_BV(MUX1)), HIGH)
#define ENABLE_ADC SET(ADCSRA, _BV(ADEN), HIGH)
#define AUTO_TRIGGER_ADC SET(ADCSRA, _BV(ADATE), HIGH)
#define TRIGGER_ADC_ON_COUNTER_0_COMPARE_MATCH SET(SFIOR, (_BV(ADTS1)|_BV(ADTS0)), HIGH)
#define ADC_INTERRUPT_ENABLE SET(ADCSRA, _BV(ADIE), HIGH)
#define LEFT_ALIGN_ADC_RESULT SET(ADMUX, _BV(ADLAR), HIGH)

// 1.3 / 2.56 * 255 = 129 = 0x81
#define BATTERY_LOW_THRESHOLD 0x81


#endif /* ADC_H_ */
