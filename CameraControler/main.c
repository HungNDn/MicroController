#include "main.h"
#include "uart_comm.h"
#include "task_scheduler.h"
#include <string.h>

void IWDG_Config(void) {
    // Khởi tạo IWDG (Independent Watchdog)
    __HAL_RCC_IWDG_CLK_ENABLE();  // Bật đồng hồ cho IWDG

    IWDG->KR = 0x5555;  // Mở quyền truy cập vào các thanh ghi của IWDG
    IWDG->PR = IWDG_PR_3;  // Chọn phân chia 256 (đây là phân chia cho tốc độ Watchdog)
    IWDG->RLR = 4000;  // Thời gian trễ watchdog (4000 ms ~ 4s)
    IWDG->KR = 0xAAAA;  // Khởi động Watchdog
}

// Khởi động lại Watchdog (reset Watchdog)
void IWDG_Reload(void) {
    IWDG->KR = 0xAAAA;  // Reload Watchdog (cập nhật lại thời gian)
}

#include "main.h"

void Error_Handler(void)
{
    Send_UART_Message("Error occurred! System halted.\n");

    // Vòng lặp vô tận để dừng hệ thống
    while (1)
    {
        // Có thể thêm các hành động khắc phục hoặc giám sát hệ thống ở đây
        // Ví dụ: Tăng cường bảo mật, thử lại cấu hình, hoặc các biện pháp khác
    }
}


void SystemClock_Config(void)
{
    // Bật HSE (High Speed External) oscillator
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    
    // Cấu hình nguồn xung nhịp HSE và PLL
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;  // Chọn HSE làm nguồn xung nhịp
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;                    // Bật HSE
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                // Bật PLL
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;        // Chọn HSE làm nguồn PLL
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;                // Nhân HSE lên 9 lần (8 MHz * 9 = 72 MHz)

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)        // Kiểm tra xem HSE có hoạt động không
    {
        // Nếu cấu hình thất bại, ta có thể xử lý lỗi ở đây
        Error_Handler();
    }

    // Cấu hình bus clocks (AHB, APB1, APB2) và Flash latency
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;   // Chọn PLL làm nguồn xung nhịp chính (72 MHz)
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;          // HCLK = SYSCLK (72 MHz)
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;           // APB1 = HCLK / 2 (36 MHz)
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;           // APB2 = HCLK (72 MHz)

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        // Xử lý lỗi nếu cấu hình thất bại
        Error_Handler();
    }

    // Cấu hình Peripheral Clocks nếu cần
}


int main(void) {
    // Khởi tạo hệ thống và các phần cứng
    HAL_Init();
    SystemClock_Config();
    MX_USART1_UART_Init();
    TaskScheduler_Init();  // Khởi tạo task scheduler
    IWDG_Config();  // Khởi tạo Watchdog

    // Đăng ký các hàm callback với thời gian trễ
    AddTask(ControlLed_Auto, 100);  // Kiểm tra và điều khiển LED mỗi 100ms
    AddTask(ControlFan_Auto, 200);  // Kiểm tra và điều khiển quạt mỗi 200ms

    // Vòng lặp chính
    while (1) {
        UART_Receive_Handler();  // Kiểm tra và xử lý lệnh nhận từ UART
        TaskScheduler_Run();  // Chạy task scheduler
        IWDG_Reload();  // Refresh watchdog timer để ngăn reset
    }
}
