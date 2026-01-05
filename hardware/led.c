#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "led.h"
void BSP_led_init()
{
	GPIO_InitTypeDef gpio_initstructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	gpio_initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_initstructure.GPIO_Pin = GPIO_Pin_13;
	gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio_initstructure);
}


void BSP_led_toggle()
{
	if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13) == Bit_RESET)
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	else
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}

