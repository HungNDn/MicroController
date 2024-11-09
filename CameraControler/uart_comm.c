#include "uart_comm.h"
#include <string.h>
#include "ir_led_control.h"
#include "fan_control.h"
#include "light_sensor.h"
#include "temperature_sensor.h"

Mode mode = MODE_AUTO;

void Send_UART_Message(const char *message) {
    HAL_UART_Transmit(&huart1, (uint8_t*)message, strlen(message), 1000);
}

void Send_UART_Message(const char *message) {
    HAL_UART_Transmit(&huart1, (uint8_t*)message, strlen(message), 1000);
}

void UART_Receive_Handler(void) {
    static uint8_t cmd;

    // Nhận lệnh từ UART (mã hex)
    if (HAL_UART_Receive_IT(&huart1, &cmd, 1) == HAL_OK) {
        // Xử lý lệnh đã nhận
        Process_Command(cmd);
    }
}

void Process_Command(uint8_t cmd) {
    char buffer[50];  // Buffer để gửi phản hồi
    
    switch (cmd) {
        case 0x01:  // SET_MODE_AUTO
            mode = MODE_AUTO;  
            Send_UART_Message("Auto mode activated.\n");
            break;

        case 0x02:  // SET_MODE_MANUAL
            mode = MODE_MANUAL; 
            Send_UART_Message("Manual mode activated.\n");
            break;

        case 0x03:  // CONTROL_LED_ON
            if (mode == MODE_MANUAL) {
                IR_LED_Control(ON); 
                Send_UART_Message("IR LED turned ON.\n");
            } else {
                Send_UART_Message("Cần chuyển Mode sang Manual để thực hiện chức năng này.\n");
            }
            break;

        case 0x04:  // CONTROL_LED_OFF
            if (mode == MODE_MANUAL) {
                IR_LED_Control(OFF);  
                Send_UART_Message("IR LED turned OFF.\n");
            } else {
                Send_UART_Message("Cant excute because mode is Auto.\n");
            }
            break;

        case 0x05:  // CONTROL_FAN_ON
            if (mode == MODE_MANUAL) {
                Fan_Control(ON);  
                Send_UART_Message("Fan turned ON.\n");
            } else {
                Send_UART_Message("Cant excute because mode is Auto.\n");
            }
            break;

        case 0x06:  // CONTROL_FAN_OFF
            if (mode == MODE_MANUAL) {
                Fan_Control(OFF);  
                Send_UART_Message("Fan turned OFF.\n");
            } else {
                Send_UART_Message("Cant excute because mode is Auto.\n");
            }
            break;

        case 0x07:  // CONTROL_IR_LED_NEAR
            if (mode == MODE_MANUAL) {
                ControlIrLed_Near();  
                Send_UART_Message("IR LED set to NEAR.\n");
            } else {
                Send_UART_Message("Cant excute because mode is Auto.\n");
            }
            break;

        case 0x08:  // CONTROL_IR_LED_FAR
            if (mode == MODE_MANUAL) {
                ControlIrLed_Far();  
                Send_UART_Message("IR LED set to FAR.\n");
            } else {
                Send_UART_Message("Cant excute because mode is Auto.\n");
            }
            break;

        case 0x09:  // GET_LED_STATE
            if (getLedState() == ON) {
                Send_UART_Message("IR LED is ON.\n");
            } else {
                Send_UART_Message("IR LED is OFF.\n");
            }
            break;

        case 0x0A:  // GET_BRIGHTNESS
            snprintf(buffer, sizeof(buffer), "Brightness: %d\n", getBrightness());
            Send_UART_Message(buffer);
            break;

        case 0x0B:  // GET_FAN_STATE
            if (getFanState() == ON) {
                Send_UART_Message("Fan is ON.\n");
            } else {
                Send_UART_Message("Fan is OFF.\n");
            }
            break;

        case 0x0C:  // READ_TEMPERATURE
            snprintf(buffer, sizeof(buffer), "Temperature: %d°C\n", Read_Temperature());
            Send_UART_Message(buffer);
            break;

        case 0x0D:  // READ_LIGHT_SENSOR
            snprintf(buffer, sizeof(buffer), "Light Level: %d\n", Read_Light_Sensor());
            Send_UART_Message(buffer);
            break;

        default:  // Lệnh không hợp lệ
            Send_UART_Message("Invalid command.\n");
            break;
    }
}
