/*
 * UART.h
 *  Author: Mynor Orantes Gonzalez
 */ 


#ifndef UART_H_
#define UART_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>

#define BAUD 9600
#define MYUBRR ((F_CPU / (BAUD * 16UL)) - 1)
#define BUFFER_SIZE 30

extern char uart_buffer[BUFFER_SIZE];
extern uint8_t buffer_index;
extern volatile uint8_t trama_lista;

void init_UART(unsigned int ubrr);
void UART_transmitir(char data);
void UART_enviar_texto(char* cadena);
void escuchar_UART(void);
void procesar_trama_serial(void);

#endif /* UART_H_ */