/*
 * Universidad del Valle de Guatemala
 * Proyecto_Brazo.c
 *
 * Created: 4/27/2026 4:43:59 PM
 *  Author: Mynor Orantes Gonzalez
 */ 



#include "ServoLib.h"
#include "UART.h"
#include "EEPROM.h"
#include <util/delay.h>

typedef enum {
	MODO_MANUAL,
	MODO_EEPROM,
	MODO_UART
} EstadoSistema;

EstadoSistema estado_actual = MODO_MANUAL;

int main(void) {
	uint16_t j1, j2, j3, j4;

	init_all_servos();
	init_ADC();
	init_UART(MYUBRR);

	ang1 = 90; ang2 = 90; ang3 = 90; ang4 = 90;
	actualizar_servos();
	actualizar_leds_modo(MODO_MANUAL);
	_delay_ms(600);

	cargar_contexto_eeprom();
	UART_enviar_texto("Sistema Inicializado Modularmente. Modo Manual Activo.\r\n");

	while (1) {
		// Cambio de modos (Pulsador PC2)
		if ((PINC & (1 << PC2)) == 0) {
			_delay_ms(50);
			if ((PINC & (1 << PC2)) == 0) {
				if (estado_actual == MODO_MANUAL) {
					estado_actual = MODO_EEPROM;
					UART_enviar_texto("Cambio a: MODO EEPROM\r\n");
					} else if (estado_actual == MODO_EEPROM) {
					estado_actual = MODO_UART;
					UART_enviar_texto("Cambio a: MODO UART (Adafruit IO)\r\n");
					} else if (estado_actual == MODO_UART) {
					estado_actual = MODO_MANUAL;
					UART_enviar_texto("Cambio a: MODO MANUAL\r\n");
				}
				
				actualizar_leds_modo(estado_actual);
				paso_reproduccion_actual = 0;
				buffer_index = 0;
				trama_lista = 0;
				
				while ((PINC & (1 << PC2)) == 0);
				_delay_ms(50);
			}
		}

		escuchar_UART();

		switch(estado_actual) {
			
			case MODO_MANUAL:
			j3 = leer_ADC(0); j4 = leer_ADC(1);
			j1 = leer_ADC(3); j2 = leer_ADC(4);

			if (j1 > 600) { ang1 += 1; if (ang1 > 180) ang1 = 180; }
			else if (j1 < 400) { ang1 -= 1; if (ang1 < 0) ang1 = 0; }

			if (j2 > 600) { ang2 += 1; if (ang2 > 180) ang2 = 180; }
			else if (j2 < 400) { ang2 -= 1; if (ang2 < 0) ang2 = 0; }

			if (j3 > 600) { ang3 += 1; if (ang3 > 180) ang3 = 180; }
			else if (j3 < 400) { ang3 -= 1; if (ang3 < 0) ang3 = 0; }

			if (j4 > 600) { ang4 += 1; if (ang4 > 180) ang4 = 180; }
			else if (j4 < 400) { ang4 -= 1; if (ang4 < 0) ang4 = 0; }

			actualizar_servos();

			// Guardar paso (Pulsador PC5)
			if ((PINC & (1 << PC5)) == 0) {
				_delay_ms(50);
				if ((PINC & (1 << PC5)) == 0) {
					guardar_paso_actual();
					while ((PINC & (1 << PC5)) == 0);
					_delay_ms(50);
				}
			}
			break;

			case MODO_EEPROM:
			reproducir_paso_eeprom();
			break;

			case MODO_UART:
			if (trama_lista) {
				procesar_trama_serial();
				buffer_index = 0;
				trama_lista = 0;
			}
			break;
		}

		_delay_ms(20);
	}
	return 0;
}