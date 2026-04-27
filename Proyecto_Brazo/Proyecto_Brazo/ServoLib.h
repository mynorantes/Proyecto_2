#ifndef SERVOLIB_H_
#define SERVOLIB_H_

#include <avr/io.h>

// Inicializa el Timer 1 para dos servos (Pines PB1 y PB2)
void initServoPWM(void);

// Funciones para mover los servos (reciben valor de 0 a 1023 del ADC)
void setServo1(uint16_t adc_val);
void setServo2(uint16_t adc_val);

#endif