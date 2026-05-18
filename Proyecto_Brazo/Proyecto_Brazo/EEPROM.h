/*
 * EEPROM.h
 *  Author: Mynor Orantes Gonzalez
 */ 

#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/io.h>

#define MAX_PASOS 4          // Límite estricto de 4 movimientos
#define DIRECCION_CONTEO 0   // Guarda cuántos pasos válidos hay en total (de 0 a 4)
#define DIRECCION_INDICE 1   // NUEVO: Guarda en qué posición toca escribir el siguiente paso
#define DIRECCION_PASOS 2    // Base donde empiezan los datos de los servos

extern uint8_t total_pasos_guardados;
extern uint8_t paso_reproduccion_actual;
extern uint8_t indice_escritura_actual; // Puntero circular

void cargar_contexto_eeprom(void);
void guardar_paso_actual(void);
void reproducir_paso_eeprom(void);

#endif /* EEPROM_H_ */