#ifndef __UART_COMM_H
#define __UART_COMM_H

#include "stm32f1xx_hal.h"
#include "usart.h"

typedef enum {
    MODE_MANUAL = 0,  
    MODE_AUTO = 1     
} Mode;

extern Mode mode;

void UART_Receive_Handler(void);
void Send_UART_Message(const char *message);

#endif /* __UART_COMM_H */
