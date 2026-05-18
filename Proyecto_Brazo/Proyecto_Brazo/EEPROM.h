/*
 * EEPROM.h
 *  Author: Mynor Orantes Gonzalez
 */ 

#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/eeprom.h>
#include <stdint.h>

// Definición de direcciones fijas en la EEPROM para cada servo
#define ADDR_SERVO1 0x10
#define ADDR_SERVO2 0x20
#define ADDR_SERVO3 0x30
#define ADDR_SERVO4 0x40

// Declaración de funciones mías
void guardar_posicion(uint8_t direccion, uint8_t angulo);
uint8_t leer_posicion(uint8_t direccion);

#endif /* EEPROM_H_ */