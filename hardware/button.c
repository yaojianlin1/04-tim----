/**
 * @file button.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-15
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
void BSP_button_init(void)
{
    GPIO_InitTypeDef gpio_initstructure;
    EXTI_InitTypeDef exti_initstructure;
    NVIC_InitTypeDef nvic_initstructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    gpio_initstructure.GPIO_Mode = GPIO_Mode_IPU;
    gpio_initstructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_10;
    gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&gpio_initstructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);

    exti_initstructure.EXTI_Line = EXTI_Line0|EXTI_Line10;
    exti_initstructure.EXTI_LineCmd = ENABLE;
    exti_initstructure.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_initstructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_Init(&exti_initstructure);


    nvic_initstructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    nvic_initstructure.NVIC_IRQChannelCmd = ENABLE;
    nvic_initstructure.NVIC_IRQChannelPreemptionPriority = 1;
    nvic_initstructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&nvic_initstructure);

    nvic_initstructure.NVIC_IRQChannel = EXTI0_IRQn;
    nvic_initstructure.NVIC_IRQChannelCmd = ENABLE;
    nvic_initstructure.NVIC_IRQChannelPreemptionPriority = 2;
    nvic_initstructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&nvic_initstructure);

}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line10)==SET)
    {
        Delay_ms(20);
        while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == Bit_RESET);
        Delay_ms(20);
        BSP_ledc13_toggle();
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
}

void EXTI0_IRQHandler(void)
{
    Delay_ms(20);
    while (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_0) == Bit_RESET);
    Delay_ms(20);
    BSP_led_stream();
    EXTI_ClearITPendingBit(EXTI_Line0);
}





