#include "ir_led_control.h"
#include "light_sensor.h"
#include <string.h>

void IR_LED_Control(State state) {
    if (state == ON) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);  
    } else {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    }
    if(state != irLedState)
        irLedState = state;
}

void ControlLed_Auto(void) {
    uint16_t lightIntensity = Read_Light_Sensor();  

    if (lightIntensity < 1000) {
        IR_LED_Control(ON);  
    } else {
        IR_LED_Control(OFF); 
    }
}

State getLedState() {
    return irLedState;
}

uint16_t getBrightness() {
    return irLedBrightness;
}

// Thiết lập độ sáng của IR LED
void IR_LED_SetBrightness(uint16_t brightness) {
    // Điều chỉnh độ sáng của IR LED thông qua PWM hoặc phương pháp phù hợp
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, brightness);  // Giả sử PWM điều khiển ở TIM2 Channel 1
}

void ControlIrLed_Near(void) {
    if (irLedState == ON) {
        if (irLedBrightness > 10) {
            irLedBrightness -= 10;  
        }
        IR_LED_SetBrightness(irLedBrightness); 
    }
}

void ControlIrLed_Far(void) {
    if (irLedState == ON) {
        if (irLedBrightness < 5000) {  
            irLedBrightness += 10; 
        }
        IR_LED_SetBrightness(irLedBrightness);  
    } 
}


