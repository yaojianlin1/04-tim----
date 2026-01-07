#include "stm32f10x.h"              
#include <stdio.h>
#include "delay.h"
#include "led.h"

#include "OLED.h"
#include "USART.h"
#include "tim.h"

void Init(void);
void ALL_Array(void);
char sss[20];


int main(void){
    Delay_ms(1000);
    Init();
    OLED_Clear();

    

    OLED_ShowString(1,1,"distance:");
    while(1){
        
    
        
        while(1){
            
            Delay_ms(10);
        }
        
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
        sprintf(sss,"rx_buffer[%d]:%d\n",i,rx_buffer1[i]);
        Serial_SendString(sss);
    }
}













