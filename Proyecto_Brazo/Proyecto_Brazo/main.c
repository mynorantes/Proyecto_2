/*
 * Universidad del Valle de Guatemala
 * Proyecto_Brazo.c
 *
 * Created: 4/27/2026 4:43:59 PM
 *  Author: Mynor Orantes Gonzalez
 */ 



#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

// Variables globales para retener la posición actual de cada servo (0 a 180)
// Iniciamos las variables en 90 para que sea el punto de partida tras un RESET
int16_t ang1 = 90;
int16_t ang2 = 90;
int16_t ang3 = 90;
int16_t ang4 = 90;

// Inicialización de los Timers para PWM
void init_all_servos(void) {
	// Configurar pines como salidas: PB1, PB2, PB3 y PD3
	DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);
	DDRD |= (1 << PD3);

	// Timer 1 (16 bits) - Modo 14 (Fast PWM con ICR1 como TOP)
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler de 8
	ICR1 = 39999; // Frecuencia de 50Hz para servos

	// Timer 2 (8 bits) - Modo Fast PWM
	TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
	TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); // Prescaler de 1024
}

// Inicialización del Convertidor Analógico-Digital (ADC)
void init_ADC(void) {
	ADMUX = (1 << REFS0); // Voltaje de referencia en AVCC (5V)
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 128
}

// Función para leer canales del ADC
uint16_t leer_ADC(uint8_t canal) {
	ADMUX = (ADMUX & 0xF0) | (canal & 0x0F);
	ADCSRA |= (1 << ADSC); // Iniciar conversión
	while (ADCSRA & (1 << ADSC)); // Esperar a que termine
	return ADC;
}

// Funciones de mapeo matemático directo a los registros OCR
void actualizar_servos(void) {
	// Timer 2 (8 bits): Rango aproximado de 6 (0°) a 32 (180°). 19 es aprox 90°
	OCR2B = 6 + (ang1 / 7);  // Servo 1 (PD3)
	OCR2A = 6 + (ang2 / 7);  // Servo 2 (PB3)

	// Timer 1 (16 bits): Rango de 2000 (0°) a 4000 (180°). 3000 es 90°
	OCR1B = 2000 + (ang3 * 11.11); // Servo 3 (PB2)
	OCR1A = 2000 + (ang4 * 11.11); // Servo 4 (PB1)
}

int main(void) {
	// 1. Inicializar Hardware
	init_all_servos();
	init_ADC();

	// 2. EFECTO RESET: Forzar los servos a 90 grados al arrancar.
	// Como ang1...ang4 valen 90 de forma global, esto se ejecuta SOLO UNA VEZ.
	actualizar_servos();
	
	// Una pequeña pausa para darle tiempo a los motores de llegar a "Home" antes de leer los mandos
	_delay_ms(500);

	// 3. BUCLE DE CONTROL EN VIVO
	while (1) {
		// Leer joysticks (Valores de 0 a 1023. Centro teórico ~512)
		uint16_t j1 = leer_ADC(3); // Joystick 1 - Eje X (Controla Servo 1)
		uint16_t j2 = leer_ADC(4); // Joystick 1 - Eje Y (Controla Servo 2)
		uint16_t j3 = leer_ADC(0); // Joystick 2 - Eje X (Controla Servo 3)
		uint16_t j4 = leer_ADC(1); // Joystick 2 - Eje Y (Controla Servo 4)

		// --- CONTROL ACUMULATIVO (RETENCIÓN DE POSICIÓN) ---
		
		// Servo 1: Modifica el ángulo solo si empujas la palanca fuera de la zona muerta
		if (j1 > 600) {
			ang1 += 1;
			if (ang1 > 180) ang1 = 180;
			} else if (j1 < 400) {
			ang1 -= 1;
			if (ang1 < 0) ang1 = 0;
		}

		// Servo 2
		if (j2 > 600) {
			ang2 += 1;
			if (ang2 > 180) ang2 = 180;
			} else if (j2 < 400) {
			ang2 -= 1;
			if (ang2 < 0) ang2 = 0;
		}

		// Servo 3
		if (j3 > 600) {
			ang3 += 1;
			if (ang3 > 180) ang3 = 180;
			} else if (j3 < 400) {
			ang3 -= 1;
			if (ang3 < 0) ang3 = 0;
		}

		// Servo 4
		if (j4 > 600) {
			ang4 += 1;
			if (ang4 > 180) ang4 = 180;
			} else if (j4 < 400) {
			ang4 -= 1;
			if (ang4 < 0) ang4 = 0;
		}

		// 4. Aplicar los ángulos calculados a los Timers en tiempo real
		actualizar_servos();

		// Este delay controla la velocidad de respuesta del brazo.
		// Si sientes que se mueve muy lento al dejar presionado el joystick, bájalo a 10ms o 15ms.
		_delay_ms(20);
	}

	return 0;
}