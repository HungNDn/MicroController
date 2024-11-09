#ifndef __FAN_CONTROL_H
#define __FAN_CONTROL_H

#include "stm32f1xx_hal.h"
#include "define.h"

State fanState = OFF; 

State getFanState();
void Fan_Control(State state);
void ControlFan_Auto(void);

#endif /* __FAN_CONTROL_H */
