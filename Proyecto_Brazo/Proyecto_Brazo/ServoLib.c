/*
 * ServoLib.c
 *  Author: Mynor Orantes Gonzalez
 */ 

#include "ServoLib.h"

int16_t ang1 = 90;
int16_t ang2 = 90;
int16_t ang3 = 90;
int16_t ang4 = 90;

void init_all_servos(void) {
	DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);
	DDRD |= (1 << PD3);
	DDRD |= (1 << PD5) | (1 << PD6) | (1 << PD7);

	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
	ICR1 = 39999;

	TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
	TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
}

void init_ADC(void) {
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	DDRC &= ~((1 << PC2) | (1 << PC5));
	PORTC |= (1 << PC2) | (1 << PC5);
}

uint16_t leer_ADC(uint8_t canal) {
	ADMUX = (ADMUX & 0xF0) | (canal & 0x0F);
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

void actualizar_servos(void) {
	OCR2B = 10 + ((ang1 * 22) / 180);
	OCR2A = 10 + ((ang2 * 22) / 180);
	OCR1B = 2000 + (ang3 * 11.11);
	OCR1A = 2000 + (ang4 * 11.11);
}

void actualizar_leds_modo(uint8_t estado) {
	PORTD &= ~((1 << PD5) | (1 << PD6) | (1 << PD7));
	if (estado == 0) PORTD |= (1 << PD5);
	else if (estado == 1) PORTD |= (1 << PD6);
	else if (estado == 2) PORTD |= (1 << PD7);
}