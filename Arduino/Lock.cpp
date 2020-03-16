/*
File:           Lock.cpp - Exercise6aState program
Author:         Jimi Österholm, Teemu Kukko & Tuomas Maanpää
Description:    Arduino program for digital lock. Control lock status and takes
				user input for new password. 
*/

#include <Arduino.h>
#include "Servo.h"
#include "LiquidCrystal.h"
#include "ButtonControl.h"
#include "Password.h"
#include "EEPROM.h"
#include "Lock.h"

// Macros for pins
#define NUMBER1 A0
#define NUMBER2 A1
#define NUMBER3 A2
#define NUMBER4 A3
#define OPEN_LOCK A4
#define SET_LOCK A5
#define RED_LED 10
#define GREEN_LED 13
#define CLOSE_LOCK 7
#define SERVO 6
#define RS 12
#define ENABLE 11
#define D4 5
#define D5 4
#define D6 3
#define D7 2

volatile lockData lock;

Servo servo;
// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(RS, ENABLE, D4, D5, D6, D7);

void setup() {
	Serial.begin(9600);
	pinMode(NUMBER1, INPUT_PULLUP);
	pinMode(NUMBER2, INPUT_PULLUP);
	pinMode(NUMBER3, INPUT_PULLUP);
	pinMode(NUMBER4, INPUT_PULLUP);
	pinMode(OPEN_LOCK, INPUT_PULLUP);
	pinMode(SET_LOCK, INPUT_PULLUP);
	pinMode(RED_LED, OUTPUT);
	pinMode(GREEN_LED, OUTPUT);
	pinMode(CLOSE_LOCK, INPUT_PULLUP);
	servo.attach(SERVO);
	EEPROM.get(0, lock.pwd);
	lockStartup("     LOCKED     ");
}

void loop() {
	
	if (lock.lockS == '2')
	{
		authenticationState(&lock);
		while (lock.lockS == '2' && lock.error < 3)
		{
			if (digitalRead(OPEN_LOCK) == LOW)
			{
				openLock('S', servo, &lock);
			}
			if (digitalRead(NUMBER1) == LOW)
			{
				increaseNumber1(lcd, &lock);
			}
			if (digitalRead(NUMBER2) == LOW)
			{
				increaseNumber2(lcd, &lock);
			}
			if (digitalRead(NUMBER3) == LOW)
			{
				increaseNumber3(lcd, &lock);
			}
			if (digitalRead(NUMBER4) == LOW)
			{
				increaseNumber4(lcd, &lock);
			}
		}
	}
	else if (lock.error < 3)
	{
		if (digitalRead(NUMBER1) == LOW)
		{
			increaseNumber1(lcd, &lock);
		}
		if (digitalRead(NUMBER2) == LOW)
		{
			increaseNumber2(lcd, &lock);
		}
		if (digitalRead(NUMBER3) == LOW)
		{
			increaseNumber3(lcd, &lock);
		}
		if (digitalRead(NUMBER4) == LOW)
		{
			increaseNumber4(lcd, &lock);
		}
		if (digitalRead(OPEN_LOCK) == LOW)
		{
			openLock('A', servo, &lock);
		}
		if (digitalRead(SET_LOCK) == LOW)
		{
			if (setLock(servo, &lock))
			{
				EEPROM.put(0, lock.pwd);
				delay(1000);
			}
		}
		if (digitalRead(CLOSE_LOCK) == LOW)
		{
			setLock2(servo, &lock);
			EEPROM.get(0, lock.pwd);
		}
	}
	else
	{
		passwordFailure(&lock);
		while (lock.lockS == 'L' && lock.error > 2 && lock.error < 4)
		{
			if (digitalRead(OPEN_LOCK) == LOW)
			{
				openLock('A', servo, &lock);
			}
			if (digitalRead(NUMBER1) == LOW)
			{
				increaseNumber1(lcd, &lock);
			}
			if (digitalRead(NUMBER2) == LOW)
			{
				increaseNumber2(lcd, &lock);
			}
			if (digitalRead(NUMBER3) == LOW)
			{
				increaseNumber3(lcd, &lock);
			}
			if (digitalRead(NUMBER4) == LOW)
			{
				increaseNumber4(lcd, &lock);
			}
		}
	}
}

/* Initialize numbers, and text to LCD */
void lockStartup(char test[50])
{
	lcd.begin(16, 2);
	lcd.setCursor(0, 0);
	lcd.print(test);
	lcd.setCursor(6, 1);
	lcd.print(0);
	lcd.setCursor(7, 1);
	lcd.print(0);
	lcd.setCursor(8, 1);
	lcd.print(0);
	lcd.setCursor(9, 1);
	lcd.print(0);
}

