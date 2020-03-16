/*
 * ButtonControl.cpp
 *
 *  Created on: 12.3.2020
 *      Author: jimi-o
 */

#include "ButtonControl.h"
#include "Lock.h"
#include "Password.h"
#include <Arduino.h>
#include "Servo.h"
#include "LiquidCrystal.h"

/* The function checks if the lock is already open and informs the user.
If the lock is closed it checks the current input using pwdCheck() function.
If the pwdCheck() returns true the lock will open. Else it will stay closed. */
void openLock(char stateLetter, Servo myservo, volatile struct lockData * lockD)
{
	if (lockD->lockS == 'O')
	{
		lockStartup("  ALREADY OPEN");
		delay(500);
		lockStartup("      OPEN     ");
		delay(500);
	}
	else
	{
		lockD->test[0] = lockD->lcd[0];
		lockD->test[1] = lockD->lcd[1];
		lockD->test[2] = lockD->lcd[2];
		lockD->test[3] = lockD->lcd[3];
		if (stateLetter == 'A')
		{
			bool answer = pwdCheck(stateLetter, lockD);
			if (answer == true)
			{
				lockStartup("CORRECT PASSWORD");
				delay(1000);
				lockD->lockS = '2';
				lockStartup("WAITING  2-STATE");
			}
			else
			{
				digitalWrite(10, HIGH);
				digitalWrite(13, LOW);
				lockStartup(" WRONG PASSWORD");
				delay(1000);
				lockD->lockS = 'L';
				lockStartup("     LOCKED     ");
			}
		}
		else if (stateLetter == 'S')
		{
			bool answer = pwdCheck(stateLetter, lockD);
			if (answer == true)
			{
				digitalWrite(13, HIGH);
				digitalWrite(10, LOW);
				lockStartup("CORRECT PASSWORD");
				myservo.write(110);
				lockD->lockS = 'O';
				delay(1000);
				lockStartup("      OPEN     ");
			}
			else
			{
				digitalWrite(10, HIGH);
				digitalWrite(13, LOW);
				lockStartup(" WRONG PASSWORD");
				delay(1000);
				lockStartup("WAITING  2-STATE");
				if (lockD->error > 2)
				{
					lockD->lockS = 'L';
				}
			}
		}
	}
	numberReset(lockD);
}

/* The function checks that the lock is open and sets the current input
as the new password. If the lock is not open, it will stay locked. */
bool setLock(Servo myservo, volatile struct lockData * lockD)
{
	if (lockD->lockS == 'O')
	{
		pwdSet(lockD);
		lockD->lockS = 'L';
		lockStartup("     LOCKED     ");
		myservo.write(30);
		numberReset(lockD);
		return true;
	}
	else
	{
		lockStartup("LOCK  IS  CLOSED");
		delay(1000);
		lockStartup("     LOCKED     ");
		numberReset(lockD);
		return false;
	}
}

/* The function checks that the lock is open and sets the current input
as the new password. If the lock is not open, it will stay locked. */
void setLock2(Servo myservo, volatile struct lockData * lockD)
{
	if (lockD->lockS == 'O')
	{
		lockD->lockS = 'L';
		lockStartup("     LOCKED     ");
		myservo.write(30);
	}
	else
	{
		lockStartup("LOCK  IS  CLOSED");
		delay(1000);
		lockStartup("     LOCKED     ");
	}
	numberReset(lockD);
}

// All the functions below are just for incrementing the numbers.

void increaseNumber1(LiquidCrystal lcd, volatile struct lockData * lockD)
{
        if (lockD->lcd[0] < 9)
        {
            lcd.setCursor(6,1);
            lcd.print(lockD->lcd[0] += 1);
        }
        else
        {
            lockD->lcd[0] = 0;
            lcd.setCursor(6,1);
            lcd.print(lockD->lcd[0]);
        }
    delay(500);
}

void increaseNumber2(LiquidCrystal lcd, volatile struct lockData * lockD)
{
        if (lockD->lcd[1] < 9)
        {
            lcd.setCursor(7,1);
            lcd.print(lockD->lcd[1] += 1);
        }
        else
        {
            lockD->lcd[1] = 0;
            lcd.setCursor(7,1);
            lcd.print(lockD->lcd[1]);
        }
    delay(500);
}

void increaseNumber3(LiquidCrystal lcd, volatile struct lockData * lockD)
{
        if (lockD->lcd[2] < 9)
        {
            lcd.setCursor(8,1);
            lcd.print(lockD->lcd[2] += 1);
        }
        else
        {
            lockD->lcd[2] = 0;
            lcd.setCursor(8,1);
            lcd.print(lockD->lcd[2]);
        }
    delay(500);
}

void increaseNumber4(LiquidCrystal lcd, volatile struct lockData * lockD)
{
        if (lockD->lcd[3] < 9)
        {
            lcd.setCursor(9,1);
            lcd.print(lockD->lcd[3] += 1);
        }
        else
        {
            lockD->lcd[3] = 0;
            lcd.setCursor(9,1);
            lcd.print(lockD->lcd[3]);
        }
    delay(500);
}

// Reset numbers back to 0
void numberReset(volatile struct lockData * lockD)
{
	lockD->lcd[0] = 0;
	lockD->lcd[1] = 0;
	lockD->lcd[2] = 0;
	lockD->lcd[3] = 0;
}
