/*
 * uart.c
 *
 *  Created on: Dec 29, 2023
 *      Author: quoct
 */


#include "uart.h"

uint8_t byte_recv = 0;
uint8_t buffer_byte[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;
uint8_t msg[100];

void uart_init_rs232()
{
    HAL_UART_Receive_IT(&huart1, &byte_recv, 1);
}

void uart_Rs232SendString(uint8_t *str)
{
    HAL_UART_Transmit(&huart1, (void *)msg, sprintf((void *)msg, "%s", str), 10);
}

void uart_Rs232SendBytes(uint8_t *bytes, uint16_t size)
{
    HAL_UART_Transmit(&huart1, bytes, size, 10);
}

void uart_Rs232SendNum(uint32_t num)
{
    if (num == 0)
    {
        uart_Rs232SendString("0");
        return;
    }
    uint8_t num_flag = 0;
    int i;
    if (num < 0)
        uart_Rs232SendString("-");
    for (i = 10; i > 0; i--)
    {
        if ((num / mypow(10, i - 1)) != 0)
        {
            num_flag = 1;
            sprintf((void *)msg, "%d", num / mypow(10, i - 1));
            uart_Rs232SendString(msg);
        }
        else
        {
            if (num_flag != 0)
                uart_Rs232SendString("0");
        }
        num %= mypow(10, i - 1);
    }
}

void uart_Rs232SendNumPercent(uint32_t num)
{
    sprintf((void *)msg, "%ld", num / 100);
    uart_Rs232SendString(msg);
    uart_Rs232SendString(".");
    sprintf((void *)msg, "%ld", num % 100);
    uart_Rs232SendString(msg);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        // rs232 isr
        // can be modified
        // HAL_UART_Transmit(&huart1, &byte_recv, 1, 10);
        buffer_byte[index_buffer] = byte_recv;
        index_buffer++;
        if (index_buffer == MAX_BUFFER_SIZE)
            index_buffer = 0;
        buffer_flag = 1;
        // turn on the receice interrupt
        HAL_UART_Receive_IT(&huart1, &byte_recv, 1);
    }
}
