/**
 * @file buzzer.case
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "stm32f10x.h"
//buzzer用的PA9
#include "buzzer.h"
// #define buzzer_pin GPIO_Pin_9
void BSP_buzzer_init(void)
{
    GPIO_InitTypeDef gpio_initstructure;
    gpio_initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio_initstructure.GPIO_Pin = buzzer_pin;
    gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio_initstructure);
}



