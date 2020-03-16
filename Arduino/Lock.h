/*
 * Lock.h
 *
 *  Created on: 12.3.2020
 *      Author: jimi-o
 */

#ifndef LOCK_H_
#define LOCK_H_

// Initialize LCD with numbers and text
void lockStartup(char test[50]);

struct lockData {
	int error = 0;
	int lcd[4] = {0,0,0,0};
	int pwd[4] = {0,0,0,0};
	int test[4] = {0,0,0,0};
	int state[4] = {0,0,0,0};
	char lockS = 'L';
};

#endif /* LOCK_H_ */
