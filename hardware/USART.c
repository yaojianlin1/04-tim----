#include "stm32f10x.h"
#include <stdio.h>
#include "USART.h"
#include "delay.h"
#include <string.h>
/**
 * PA9   TX
 * PA10  RX
 * USART1 --- tof
 * 
 * PA2   TX
 * PA3   RX
 * USART2 --- USB to TTL
 */

uint8_t rx_buffer[BUFFER_SIZE];
uint8_t tx_buffer[BUFFER_SIZE+3];
static uint8_t count = 0;

volatile uint16_t write_rx_buffer = 0;
volatile uint8_t rx_complete_flag = 0;



void Serial_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    tx_buffer[0] = 'd';
    tx_buffer[1] = ':';


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOA,&GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = USART_BAUDRATE;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1,&USART_InitStructure);
    USART_Init(USART2,&USART_InitStructure);



    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    //开启RXNE中断和IDLE中断，每来一个字节就在中断程序中将他转移到rx_buffer中，IDLE中断就置标志位为1
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);



    USART_Cmd(USART1,ENABLE);
    USART_Cmd(USART2,ENABLE);
    
    USART_ClearFlag(USART1,USART_FLAG_TC);

}

void DMAUSART_Init(void){

    DMA_InitTypeDef DMA_InitStructure;
    

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    DMA_DeInit(DMA1_Channel7);
    DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE+2;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//内存到外设
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)tx_buffer;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_Init(DMA1_Channel7,&DMA_InitStructure);

    DMA_Cmd(DMA1_Channel7,DISABLE);

}


void Serial_send(uint8_t Byte){

    while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
    USART_SendData(USART2,Byte);
    
}

void Serial_SendString(char *s){
    uint8_t i = 0;
    for(i = 0; s[i] != '\0';i++){
        Serial_send(s[i]);
    }
}


static void USART2_send_DMA(){
    uint8_t i = 0;
    DMA_Cmd(DMA1_Channel7,DISABLE);
    DMA_ClearFlag(DMA1_FLAG_TC7 | DMA1_FLAG_GL7 | DMA1_FLAG_HT7 | DMA1_FLAG_TE7);
    DMA_SetCurrDataCounter(DMA1_Channel7,write_rx_buffer+3);
    
    for(i = write_rx_buffer;i<BUFFER_SIZE;i++){
        rx_buffer[i] = '\0'; 
    }
    for(i = 0;i<write_rx_buffer;i++){
        tx_buffer[i+2] = rx_buffer[i];
    }
    tx_buffer[write_rx_buffer+2] = '\n';
    tx_buffer[write_rx_buffer+3] = '\0';
    USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
    DMA_Cmd(DMA1_Channel7,ENABLE);
}


void USART2_DMA_disable(){
    
    USART_DMACmd(USART2,USART_DMAReq_Tx,DISABLE);
    DMA_Cmd(DMA1_Channel7,DISABLE);
}

void USART1_IRQHandler(void){
    uint16_t tmp = 0;
    if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET){
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
        tmp = USART_ReceiveData(USART1);
        //如果tmp是数字就接收，否则就丢弃
        if(tmp >= '0' && tmp <= '9'){
            rx_buffer[write_rx_buffer++] = tmp;
        }
    }

    if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET){
        count++;
        USART_ReceiveData(USART1);
        USART_ClearITPendingBit(USART1,USART_IT_IDLE);
        if(count >= 10){
            USART2_send_DMA();
            count = 0;
            rx_complete_flag = 1;
        }
        
        
        write_rx_buffer = 0;
    }
}



