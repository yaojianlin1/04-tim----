#ifndef __USART_H
#define __USART_H

#define USART_BAUDRATE 115200

#define BUFFER_SIZE 4

extern uint8_t rx_buffer1[BUFFER_SIZE+2];




extern volatile uint8_t rx_complete_flag;

extern volatile uint16_t write_rx_buffer;


void Serial_Init(void);
void DMAUSART_Init(void);
void Serial_send(uint8_t Byte);
void Serial_SendString(char *s);




#endif

