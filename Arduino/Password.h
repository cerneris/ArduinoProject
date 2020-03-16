/*
 * Password.h
 *
 *  Created on: 12.3.2020
 *      Author: jimi-o
 */
#include "Servo.h"

#ifndef PASSWORD_H_
#define PASSWORD_H_

void authenticationState(volatile struct lockData * lockd);
void passwordFailure(volatile struct lockData * lockD);
// Checking if the test password is the same as the set password
// Set the new password
void pwdSet(volatile struct lockData * lockD);
bool pwdCheck(char state, volatile struct lockData * lockD);

#endif /* PASSWORD_H_ */
