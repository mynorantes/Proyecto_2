/*
 * UART.c
 *  Author: Mynor Orantes Gonzalez
 */ 



#include "UART.h"
#include "ServoLib.h" // Enlace para actualizar los ángulos globales directamente
#include <stdio.h>

// Definición de variables globales compartidas declaradas en el header
char uart_buffer[BUFFER_SIZE];
uint8_t buffer_index = 0;
volatile uint8_t trama_lista = 0;

// Inicializa el módulo USART0 del ATmega328P
void init_UART(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	
	// Habilitar receptor (RX) y transmisor (TX)
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	
	// Configurar formato de trama: 8 datos, 1 bit de parada, sin paridad
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Envía un solo carácter físico por el canal TX
void UART_transmitir(char data) {
	// Esperar a que el buffer de transmisión esté vacío
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

// Envía una cadena de texto completa (string) terminada en '\0'
void UART_enviar_texto(char* cadena) {
	while (*cadena) {
		UART_transmitir(*cadena++);
	}
}

// Captura de forma asíncrona y blindada los caracteres entrantes en el main loop
void escuchar_UART(void) {
	// Comprobar si hay un byte nuevo sin leer en el registro de recepción
	if (UCSR0A & (1 << RXC0)) {
		char char_recibido = UDR0;
		
		// CASO 1: Si detecta fin de línea estándar (\n o \r) indica que la trama terminó
		if (char_recibido == '\n' || char_recibido == '\r') {
			if (buffer_index > 0) {
				uart_buffer[buffer_index] = '\0'; // Cerrar la cadena de caracteres de forma segura
				trama_lista = 1;                  // Activar bandera de procesamiento
			}
		}
		// CASO 2: Si por alguna razón Adafruit IO envía un '#' como delimitador de cierre
		else if (char_recibido == '#' && buffer_index > 4) {
			uart_buffer[buffer_index] = '\0';
			trama_lista = 1;
		}
		// CASO 3: Almacenamiento ordinario de caracteres numéricos y comas
		else {
			// Filtro de seguridad: Ignorar el '#' si viene al puro inicio para procesar directo los números
			if (!(char_recibido == '#' && buffer_index == 0)) {
				if (buffer_index < (BUFFER_SIZE - 1)) {
					uart_buffer[buffer_index++] = char_recibido;
				}
			}
		}
	}
}

// Desempaqueta la cadena de texto, valida límites y ejecuta el movimiento de los servos
void procesar_trama_serial(void) {
	int t1, t2, t3, t4;
	
	// sscanf busca estrictamente el patrón de 4 enteros separados por comas: "ANG1,ANG2,ANG3,ANG4"
	if (sscanf(uart_buffer, "%d,%d,%d,%d", &t1, &t2, &t3, &t4) == 4) {
		
		// Filtro de protección física para los servomotores (Rango válido: 0 a 180 grados)
		if(t1 >= 0 && t1 <= 180) ang1 = t1;
		if(t2 >= 0 && t2 <= 180) ang2 = t2;
		if(t3 >= 0 && t3 <= 180) ang3 = t3;
		if(t4 >= 0 && t4 <= 180) ang4 = t4;
		
		// Mapear físicamente los nuevos ángulos a los registros de los Timers (PWM)
		actualizar_servos();
		
		// Responder con un mensaje de éxito (ACK) hacia la terminal o pasarela IoT
		UART_enviar_texto("ACK: Motores posicionados correctamente\r\n");
		} else {
		// Alerta de diagnóstico en caso de recibir caracteres corruptos o mal formateados
		UART_enviar_texto("ERR: Formato invalido. Use: ANG1,ANG2,ANG3,ANG4 (Ej: 90,45,180,0)\r\n");
	}
}