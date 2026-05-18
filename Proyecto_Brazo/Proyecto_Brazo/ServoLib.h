/*
 * ServoLib.h
 *  Author: Mynor Orantes Gonzalez
 */ 


#ifndef SERVOLIB_H_
#define SERVOLIB_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>

extern int16_t ang1;
extern int16_t ang2;
extern int16_t ang3;
extern int16_t ang4;

void init_all_servos(void);
void init_ADC(void);
uint16_t leer_ADC(uint8_t canal);
void actualizar_servos(void);
void actualizar_leds_modo(uint8_t estado);

#endif /* SERVOLIB_H_ */