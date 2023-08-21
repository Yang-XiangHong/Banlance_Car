#ifndef _USART_H
#define _USART_H

#include "stm32f10x.h"
#include "stdio.h"

void uart_init(u32 bound);
void send_byte(uint8_t byte);
void send_string(uint8_t *str);
void send_buf(uint8_t *buf, uint16_t len);
void send_pack(void);

void SendData(int rec_data);

#endif
