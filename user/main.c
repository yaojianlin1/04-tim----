#include "stm32f10x.h"              
#include <stdio.h>
#include "delay.h"
#include "led.h"

#include "OLED.h"
#include "USART.h"
#include "tim.h"

void Init(void);
void ALL_Array(void);
void GetDistance(void);

uint8_t test = 0;
char s[12] = "yaojianlin\n";
char sss[20];
char tmp[20];
uint8_t *data_to_process = NULL;

int main(void){
    Delay_ms(1000);
    Init();
    OLED_Clear();

    // OLED_ShowNum(2,2,10,2);

    OLED_ShowString(1,1,"data");
    while(1){
        // Serial_SendString(s);
        // OLED_ShowNum(2,2,test++,2);
        (void)data_to_process;
        (void)tmp;
        // Serial_SendString(s);

        while(rx_complete_flag == 0);
            __disable_irq();
            rx_complete_flag = 0;
            last_length = (rx_data_length + last_length)%BUFFER_SIZE;
            GetDistance();
            OLED_ShowNum(3,1,rx_data_length,10);
            // sprintf(sss,"sss:%s",s);
            sprintf(tmp,"dis:%s",distance);
            // Serial_SendString(sss);
            Serial_SendString(distance);
            OLED_ShowString(4,1,tmp);
            __enable_irq();

        // Delay_s(3);
    }
}

void Init(void){
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    BSP_led_init();
    OLED_Init();
    Serial_Init();
    DMAUSART_Init();
}

void ALL_Array(void){
    int i = 0;
    for(i=0;i<BUFFER_SIZE;i++ ){
        Serial_send(rx_buffer1[i]);
    }
}
/**
 * @brief Get the Distance object
 * 
 * 已经有了以及DMA_GetCurrDataCounter(DMA1_Channel5)
 */
void GetDistance(void){
    uint16_t fir = 0,end = 0,i=0,j=0;
    end = BUFFER_SIZE - DMA_GetCurrDataCounter(DMA1_Channel5) - 3;//end指向'm'
    for(i = 0;i<BUFFER_SIZE;i++){
        if( rx_buffer1[(end - i + BUFFER_SIZE -1) % BUFFER_SIZE] == '\n' ){
            fir = (i+1)%BUFFER_SIZE;
            break;
        }
    }
    if(fir<end){
        for(i=fir;i<=end;i++){
            distance[j++] = rx_buffer1[i];
        }
    }else{
        for(i=fir;i<=end+BUFFER_SIZE;i++){
            distance[j++] = rx_buffer1[(i+BUFFER_SIZE)%BUFFER_SIZE];
        }
    }
    distance[j] = '\n';
}











