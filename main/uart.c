/*
 * uart.c
 *
 *  Created on: 15 wrz 2024
 *      Author: majorbien
 */

#include "esp_log.h"
#include <stdio.h>
#include <string.h>
#include "driver/uart.h"

#define TXD_PIN 21
#define RXD_PIN 22

static const char *TAG = "UART";

// UART Initialization
void uart_init() {
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_0, 1024, 0, 0, NULL, 0);
}

// UART Send Data
void uart_send_data(const char *data, size_t len) {
    uart_write_bytes(UART_NUM_0, data, len);
    ESP_LOGI("TAG","Data send: %s", data);
    
}
