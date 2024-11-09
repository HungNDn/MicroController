#include "fan_control.h"
#include "temperature_sensor.h"


void Fan_Control(State state) {
    if (state == ON) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);  
    } else {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
    }
    if(state != fanState)
        fanState = state;
}

void ControlFan_Auto(void) {
    uint16_t temperature = Read_Temperature(); 

    if (temperature > 30) {
        Fan_Control(ON);  
    } else {
        Fan_Control(OFF);  
    }
}

State getFanState() {
    return fanState;
}

