/*
 * uart.h
 *
 *  Created on: Dec 29, 2023
 *      Author: quoct
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "usart.h"
#include <stdio.h>
#include "utils.h"

#define MAX_BUFFER_SIZE 30

extern uint8_t byte_recv;
extern uint8_t buffer_byte[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;

void uart_init_rs232();

void uart_Rs232SendString(uint8_t* str);

void uart_Rs232SendBytes(uint8_t* bytes, uint16_t size);

void uart_Rs232SendNum(uint32_t num);

void uart_Rs232SendNumPercent(uint32_t num);

#endif /* INC_UART_H_ */
