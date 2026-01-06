#ifndef __USART_H
#define __USART_H

#define USART_BAUDRATE 115200

#define BUFFER_SIZE 256

extern uint8_t rx_buffer1[BUFFER_SIZE];
extern uint8_t rx_buffer2[BUFFER_SIZE];


extern volatile uint8_t *current_rx_buffer;
extern volatile uint16_t rx_data_length;
extern volatile uint8_t rx_complete_flag;
extern volatile uint8_t *ptr;
extern volatile uint16_t last_length;

extern volatile char distance[12];

void Serial_Init(void);
void DMAUSART_Init(void);
void Serial_send(uint8_t Byte);
void Serial_SendString(char *s);




#endif

