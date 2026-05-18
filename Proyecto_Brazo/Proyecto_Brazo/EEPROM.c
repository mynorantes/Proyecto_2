/*
 * EEPROM.c
 *  Author: Mynor Orantes Gonzalez
 */ 


#include "EEPROM.h"

/**
 * @brief Guarda un ángulo en una dirección específica de la EEPROM.
 * Usa eeprom_update_byte para escribir únicamente si el valor cambió,
 * protegiendo la vida útil de la memoria.
 */
void guardar_posicion(uint8_t direccion, uint8_t angulo) {
    // El cast a (uint8_t*)(uintptr_t) evita los warnings del compilador de Atmel/Microchip Studio
    eeprom_update_byte((uint8_t*)(uintptr_t)direccion, angulo);
}

/**
 * @brief Lee la posición guardada. Si la memoria está vacía tras un reset
 * o primera alimentación, inicializa y devuelve 90 grados.
 */
uint8_t leer_posicion(uint8_t direccion) {
    uint8_t valor = eeprom_read_byte((uint8_t*)(uintptr_t)direccion);
    
    // 0xFF (255) significa celda vacía/limpia de fábrica. 
    // Si es mayor a 180, es un dato inválido o basura.
    if (valor > 180) {
        // Forzamos el valor de seguridad en 90 grados
        valor = 90; 
        // Lo grabamos de una vez para dejar la EEPROM configurada correctamente
        guardar_posicion(direccion, valor);
    }
    
    return valor;
}