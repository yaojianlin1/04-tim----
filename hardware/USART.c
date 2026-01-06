#include "stm32f10x.h"
#include <stdio.h>
#include "USART.h"


/**
 * PA9   TX
 * PA10  RX
 * USART1 --- USBtoTTL
 * 
 * PA2   TX
 * PA3   RX
 * USART2
 */

uint8_t rx_buffer1[BUFFER_SIZE];
uint8_t rx_buffer2[BUFFER_SIZE];

volatile uint8_t *current_rx_buffer = rx_buffer1;
volatile uint16_t rx_data_length = 0;
volatile uint16_t last_length = 0;
volatile uint8_t *ptr = rx_buffer1;

volatile uint8_t rx_complete_flag = 0;




void Serial_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = USART_BAUDRATE;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1,&USART_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);


    USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);


    USART_Cmd(USART1,ENABLE);
    USART_ClearFlag(USART1,USART_FLAG_TC);

}

void DMAUSART_Init(void){

    DMA_InitTypeDef DMA_InitStructure;
    // NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)rx_buffer1;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_Init(DMA1_Channel5,&DMA_InitStructure);

    DMA_Cmd(DMA1_Channel5,ENABLE);

    // NVIC_InitStructure.NVIC_IRQChannel = ;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = ;
    // NVIC_Init(&NVIC_InitStructure);

}




void Serial_send(uint8_t Byte){

    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
    USART_SendData(USART1,Byte);
    
}

void Serial_SendString(char *s){
    uint8_t i = 0;
    for(i = 0; s[i] != '\0';i++){
        Serial_send(s[i]);
    }
}


void USART1_IRQHandler(void){
    if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET){
        
        USART_ReceiveData(USART1);
        DMA_Cmd(DMA1_Channel5,DISABLE);

        rx_data_length = BUFFER_SIZE - DMA_GetCurrDataCounter(DMA1_Channel5)-last_length;   
        rx_complete_flag = 1;
        

        DMA_Cmd(DMA1_Channel5,ENABLE);

    }
}
