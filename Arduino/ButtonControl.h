/*
 * ButtonControl.h
 *
 *  Created on: 12.3.2020
 *      Author: jimi-o
 */

#include "LiquidCrystal.h"
#include "Servo.h"

#ifndef BUTTONCONTROL_H_
#define BUTTONCONTROL_H_

// Test the password and open the lock if it's correct
void openLock(char stateLetter, Servo myservo, volatile struct lockData * lockD);
// Check that the lock is open and set the current numbers as password
bool setLock(Servo myservo, volatile struct lockData * lockD);
// Lock without changing password
void setLock2(Servo myservo, volatile struct lockData * lockD);
// Increment number 1
void increaseNumber1(LiquidCrystal lcd, volatile struct lockData * lockD);
// Increment number 2
void increaseNumber2(LiquidCrystal lcd, volatile struct lockData * lockD);
// Increment number 3
void increaseNumber3(LiquidCrystal lcd, volatile struct lockData * lockD);
// Increment number 4
void increaseNumber4(LiquidCrystal lcd, volatile struct lockData * lockD);
// Reset all numbers
void numberReset(volatile struct lockData * lockD);

#endif /* BUTTONCONTROL_H_ */
