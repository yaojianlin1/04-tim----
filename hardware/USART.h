#ifndef __USART_H
#define __USART_H

#define USART_BAUDRATE 115200

#define BUFFER_SIZE 5

extern uint8_t rx_buffer[BUFFER_SIZE];
extern uint8_t tx_buffer[BUFFER_SIZE+3];



extern volatile uint8_t rx_complete_flag;

extern volatile uint16_t write_rx_buffer;



void Serial_Init(void);
void DMAUSART_Init(void);
void Serial_send(uint8_t Byte);
void Serial_SendString(char *s);




#endif

