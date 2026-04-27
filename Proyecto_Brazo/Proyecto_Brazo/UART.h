/*
 * UART.h
 */ 


#ifndef UART_H_
#define UART_H_

#define F_CPU 16000000UL
#include <avr/io.h>

void init_uart(uint16_t ubrr);
void uart_transmit(char data);
void cadena(char txt[]);

#endif /* UART_H_ */