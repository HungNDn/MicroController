#ifndef __IR_LED_CONTROL_H
#define __IR_LED_CONTROL_H

#include "stm32f1xx_hal.h"
#include "define.h"

uint16_t irLedBrightness = 3000;  
State irLedState = OFF; 

uint16_t getBrightness();
State getLedState();
void IR_LED_Control(State state);
void ControlLed_Auto(void);
void IR_LED_SetBrightness(uint16_t brightness);
void ControlIrLed_Near(void);
void ControlIrLed_Far(void);



#endif /* __IR_LED_CONTROL_H */
