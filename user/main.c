#include "stm32f10x.h"              
#include <stdio.h>
#include "delay.h"
#include "led.h"

#include "OLED.h"
#include "USART.h"
#include "tim.h"

void Init(void);
uint8_t test = 0;
char s[12] = "yaojianlin\n";
uint8_t *data_to_process = NULL;

int main(void){
    Delay_ms(1000);
    Init();
    OLED_Clear();
    // OLED_ShowNum(2,2,10,2);

    OLED_ShowString(1,1,"data");
    while(1){
        // Serial_SendString(s);
        OLED_ShowNum(2,2,test++,2);
        if(rx_complete_flag == 1){
            rx_complete_flag = 0;
            data_to_process = rx_buffer1;
            OLED_ShowString(3,1,"length:");
            OLED_ShowNum(4,1,rx_data_length,10);
            Serial_SendString(data_to_process);
        }

        Delay_ms(10);
    }
}

void Init(void){
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    BSP_led_init();
    OLED_Init();
    Serial_Init();
    DMAUSART_Init();
}