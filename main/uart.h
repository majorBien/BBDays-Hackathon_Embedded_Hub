/*
 * uart.h
 *
 *  Created on: 15 wrz 2024
 *      Author: majorbien
 */

#ifndef MAIN_UART_H_
#define MAIN_UART_H_

void uart_init();

void uart_send_data(const char *data, size_t len);

#endif /* MAIN_UART_H_ */
