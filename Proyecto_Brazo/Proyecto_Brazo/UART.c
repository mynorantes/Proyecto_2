/*
 * UART.c
 *  Author: Mynor Orantes Gonzalez
 */ 

#include "UART.h"

void init_uart(uint16_t ubrr) {
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}

void uart_transmit(char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void cadena(char txt[]) {
	for (int i = 0; txt[i] != '\0'; i++) {
		uart_transmit(txt[i]);
	}
}