/*
 * EEPROM.c
 *  Author: Mynor Orantes Gonzalez
 */ 


#include "EEPROM.h"
#include "ServoLib.h"
#include <avr/eeprom.h>
#include <util/delay.h>

uint8_t total_pasos_guardados = 0;
uint8_t paso_reproduccion_actual = 0;
uint8_t indice_escritura_actual = 0;

void cargar_contexto_eeprom(void) {
	total_pasos_guardados = eeprom_read_byte((uint8_t*)DIRECCION_CONTEO);
	indice_escritura_actual = eeprom_read_byte((uint8_t*)DIRECCION_INDICE);
	
	// Validaciones de seguridad por si la EEPROM está virgen (255)
	if (total_pasos_guardados > MAX_PASOS) {
		total_pasos_guardados = 0;
	}
	if (indice_escritura_actual >= MAX_PASOS) {
		indice_escritura_actual = 0;
	}
}

void guardar_paso_actual(void) {
	// 1. Calcular la dirección física usando el índice circular actual
	uint16_t direccion = DIRECCION_PASOS + (indice_escritura_actual * 4);
	
	// 2. Sobrescribir los datos viejos con los ángulos actuales de los servos
	eeprom_write_byte((uint8_t*)(direccion + 0), (uint8_t)ang1);
	eeprom_write_byte((uint8_t*)(direccion + 1), (uint8_t)ang2);
	eeprom_write_byte((uint8_t*)(direccion + 2), (uint8_t)ang3);
	eeprom_write_byte((uint8_t*)(direccion + 3), (uint8_t)ang4);
	
	// 3. Desplazar el índice de escritura de forma circular (0 -> 1 -> 2 -> 3 -> 0...)
	indice_escritura_actual = (indice_escritura_actual + 1) % MAX_PASOS;
	eeprom_write_byte((uint8_t*)DIRECCION_INDICE, indice_escritura_actual);
	
	// 4. Incrementar el conteo de pasos totales sólo hasta llegar al tope de 4
	if (total_pasos_guardados < MAX_PASOS) {
		total_pasos_guardados++;
		eeprom_write_byte((uint8_t*)DIRECCION_CONTEO, total_pasos_guardados);
	}
	
	// Destello visual en el LED indicador (PD5) para confirmar la grabación
	PORTD &= ~(1 << PD5);
	_delay_ms(80);
	PORTD |= (1 << PD5);
}

void reproducir_paso_eeprom(void) {
	if (total_pasos_guardados > 0) {
		uint16_t direccion = DIRECCION_PASOS + (paso_reproduccion_actual * 4);
		int16_t target1 = eeprom_read_byte((uint8_t*)(direccion + 0));
		int16_t target2 = eeprom_read_byte((uint8_t*)(direccion + 1));
		int16_t target3 = eeprom_read_byte((uint8_t*)(direccion + 2));
		int16_t target4 = eeprom_read_byte((uint8_t*)(direccion + 3));
		
		// Interpolación suave hacia la posición objetivo
		while ((ang1 != target1) || (ang2 != target2) || (ang3 != target3) || (ang4 != target4)) {
			if ((PINC & (1 << PC2)) == 0) return; // Salida inmediata si cambian de modo

			if (ang1 < target1) ang1++; else if (ang1 > target1) ang1--;
			if (ang2 < target2) ang2++; else if (ang2 > target2) ang2--;
			if (ang3 < target3) ang3++; else if (ang3 > target3) ang3--;
			if (ang4 < target4) ang4++; else if (ang4 > target4) ang4--;
			
			actualizar_servos();
			_delay_ms(15);
		}
		
		// Pausa estática fija de 1 segundo en la posición alcanzada
		for (uint8_t i = 0; i < 40; i++) {
			if ((PINC & (1 << PC2)) == 0) return;
			_delay_ms(25);
		}
		
		// Avanzar al siguiente paso guardado de forma cíclica
		paso_reproduccion_actual++;
		if (paso_reproduccion_actual >= total_pasos_guardados) {
			paso_reproduccion_actual = 0;
		}
		} else {
		// Si no hay pasos guardados, el brazo se queda centrado en reposo
		ang1 = 90; ang2 = 90; ang3 = 90; ang4 = 90;
		actualizar_servos();
	}
}