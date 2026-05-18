/*
 * ServoLib.h
 *  Author: Mynor Orantes Gonzalez
 */ 


#ifndef SERVOLIB_H_
#define SERVOLIB_H_

#include <avr/io.h>

void initServoPWM(void);
void setServo1(uint16_t angle);
void setServo2(uint16_t angle);
void setServo3(uint16_t angle);
void setServo4(uint16_t angle); 

#endif