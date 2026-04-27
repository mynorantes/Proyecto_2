#include "ServoLib.h"

void initServoPWM(void) {
	// Configurar PB1 (OC1A) y PB2 (OC1B) como salidas
	DDRB |= (1 << DDB1) | (1 << DDB2);

	// Modo 14: Fast PWM con TOP en ICR1
	// COM1A1 y COM1B1: Non-inverting (limpia en comparar, set en TOP)
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler 8

	ICR1 = 39999; // Frecuencia de 50Hz (20ms)
}

void setServo1(uint16_t adc_val) {
	// Mapeo: ADC(0-1023) a Pulso(2000-4000) -> 1ms a 2ms
	OCR1A = 1000 + (adc_val * 2000UL) / 1023;
}

void setServo2(uint16_t adc_val) {
	OCR1B = 1000 + (adc_val * 2000UL) / 1023;
}