#include "stm32f10x.h"              

#include "delay.h"
#include "led.h"

#include "OLED.h"

#include "tim.h"

void Init(void);

int main(void){
    Init();
    OLED_Clear();
    OLED_ShowNum(2,2,10,2);
    while(1){
        
    }
}

void Init(void){
    BSP_led_init();
    OLED_Init();
}