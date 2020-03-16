/*
 * Password.cpp
 *
 *  Created on: 12.3.2020
 *      Author: jimi-o
 */

#include "Password.h"
#include "Lock.h"
#include <Arduino.h>
#include "ButtonControl.h"
#include "EEPROM.h"

void authenticationState(volatile struct lockData * lockD)
{
	Serial.write('1');
	Serial.flush();
	delay(1100);
	if (Serial.available() > 0)
	{
		char buffer[4];
		Serial.readBytes(buffer, 4);
		lockD->state[0] = buffer[0] - '0';
		lockD->state[1] = buffer[1] - '0';
		lockD->state[2] = buffer[2] - '0';
		lockD->state[3] = buffer[3] - '0';
		lockD->lockS = '2';
		Serial.flush();
		delay(2000);
	}
}

void passwordFailure(volatile struct lockData * lockD)
{
	Serial.write('2');
	Serial.flush();
	lockStartup("   YOU FAILED   ");
	delay(1100);
	lockD->error = 3;
	if (Serial.available() > 0)
	{
		char buffer[4];
		Serial.readBytes(buffer, 4);
		lockD->pwd[0] = buffer[0] - '0';
		lockD->pwd[1] = buffer[1] - '0';
		lockD->pwd[2] = buffer[2] - '0';
		lockD->pwd[3] = buffer[3] - '0';
		Serial.flush();
		delay(5000);
	}
}

bool pwdCheck(char state, volatile struct lockData * lockD)
{
	if (state == 'S')
	{
		for (int i = 0; i <= 3; i++)
		{
			if (lockD->state[i] == lockD->test[i])
			{
				continue;
			}
			else
			{
				numberReset(lockD);
				lockD->error++;
				return false;
			}
		}
	}
	else
	{
		for (int i = 0; i <= 3; i++)
		{
			if (lockD->pwd[i] == lockD->test[i])
			{
				continue;
			}
			else
			{
				numberReset(lockD);
				lockD->error++;
				return false;
			}
		}
	}
	lockD->error = 0;
	return true;
}

/* Set the current input password as the new password and reset numbers */
void pwdSet(volatile struct lockData * lockD)
{
	lockD->pwd[0] = lockD->lcd[0];
	lockD->pwd[1] = lockD->lcd[1];
	lockD->pwd[2] = lockD->lcd[2];
	lockD->pwd[3] = lockD->lcd[3];

	numberReset(lockD);

	lockD->test[0] = lockD->lcd[0];
	lockD->test[1] = lockD->lcd[1];
	lockD->test[2] = lockD->lcd[2];
	lockD->test[3] = lockD->lcd[3];

	digitalWrite(13, LOW);
	digitalWrite(10, LOW);
	delay(100);
	digitalWrite(13, HIGH);
	digitalWrite(10, HIGH);
	delay(100);
	digitalWrite(13, LOW);
	digitalWrite(10, LOW);
	delay(100);
	digitalWrite(13, HIGH);
	digitalWrite(10, HIGH);
	delay(100);
	digitalWrite(13, LOW);
	digitalWrite(10, LOW);
	delay(100);
}
