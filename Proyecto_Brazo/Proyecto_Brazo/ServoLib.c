/*
 * ServoLib.c
 *  Author: Mynor Orantes Gonzalez
 */ 

#include "ServoLib.h"
#include <avr/io.h>

void initServoPWM(void) {
	// Configurar salidas: PB1, PB2, PB3 y PD3
	DDRB |= (1 << DDB1) | (1 << DDB2) | (1 << DDB3);
	DDRD |= (1 << DDD3);

	// --- TIMER 1 (16 bits) ---
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
	ICR1 = 39999;

	// --- TIMER 2 (8 bits) ---
	TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
	TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
}

void setServo1(uint16_t angle) {
	if (angle == 90) { TCCR2A &= ~(1 << COM2B1); PORTD &= ~(1 << PD3); }
	else { TCCR2A |= (1 << COM2B1); OCR2B = 6 + (angle / 7); }
}

void setServo2(uint16_t angle) {
	if (angle == 90) { TCCR2A &= ~(1 << COM2A1); PORTB &= ~(1 << PB3); }
	else { TCCR2A |= (1 << COM2A1); OCR2A = 6 + (angle / 7); }
}

void setServo3(uint16_t angle) {
	if (angle == 90) { TCCR1A &= ~(1 << COM1B1); PORTB &= ~(1 << PB2); }
	else { TCCR1A |= (1 << COM1B1); OCR1B = 2000 + (angle * 11.11); }
}

void setServo4(uint16_t angle) {
	if (angle == 90) { TCCR1A &= ~(1 << COM1A1); PORTB &= ~(1 << PB1); }
	else { TCCR1A |= (1 << COM1A1); OCR1A = 2000 + (angle * 11.11); }
}