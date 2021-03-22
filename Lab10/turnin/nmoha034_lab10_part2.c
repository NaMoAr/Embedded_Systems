/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #10  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *      Demo Link: https://drive.google.com/file/d/1KD9oljMdhJiZ99LkVfTjWBoweKojeN7V/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include <keypad.h>
#include <scheduler.h>
#endif

enum LockStates {Start_Lock, Init_Lock, Lock};
int LockTick(int state) {
	switch(state) {
		case Start_Lock:
			state = Init_Lock;
			break;
		case Init_Lock:
			if ((~PINB & 0x80) == 0x80) {
				state = Lock;}
			else {
				state = Init_Lock;
			}
			break;
		case Lock:
			state = Init_Lock;
			
                        break;
		}
	switch(state) {
		case Start_Lock:
			break;
		case Init_Lock:
			break;
		case Lock:
			PORTB = 0x00;
			break;
		
	}
	return state;
}



unsigned char x = 0x00;
unsigned char i = 0x00;
unsigned char array[6] = {'#', '1' ,'2','3', '4' ,'5'};
enum UnlockStates {Start_unlock, Init_unlock, Correct, Wrong, Unlock};
int UnlockTick(int state) {
	x = GetKeypadKey();
	switch(state) {
		case Start_unlock:
			state = Init_unlock;
			break;
		case Init_unlock:
			if (x == array[i]) {
				state = Correct;
			}
			else if (x == 0x00) {
				state = Init_unlock;
			}
			else {
				state = Wrong;
			}
			break;
		case Correct:
			
			if (i == 0x05 && ((PORTB & 0x01) == 0x00)) {
				state = Unlock;
			}
			else if (x == 0x00) {
                                ++i;
				state = Init_unlock;
                        }
			else {
				state = Correct;
			}
                        break;
		case Wrong:
			if (x == 0x00) {
				i = 0x00;
				state = Init_unlock;
			}
			else {
				state = Wrong;
			}
			break;
		case Unlock:
                        if (x == 0x00) {
				state = Init_unlock;
				i = 0x00;
			}
			else {
				state = Unlock;
			}
			break;
		}
	switch(state) {
		case Start_unlock:
			PORTB = 0x01;
			break;
		case Init_unlock:
			break;
		case Correct:
			break;
		case Wrong:
			break;
		case Unlock:
			PORTB = 0x01;
			break;
	}
	return state;
}









int main() {
    /* Insert DDR and PORT initializations */
    DDRB = 0x7F; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;
    /* Insert your solution below */
    static task task1;
    static task task2;
    task *tasks[] = { &task1 , &task2};
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    unsigned long GCD  = tasks[0]->period;
    const char start = 0;
    task1.state = start;
    task1.period = 50;
    task1.elapsedTime = task1.period;
    task1.TickFct = &UnlockTick;

    task2.state = start;
    task2.period = 50;
    task2.elapsedTime = task2.period;
    task2.TickFct = &LockTick;
    

    TimerSet(50);
    TimerOn();


	

    unsigned short i;
    while (1) {
	for (i = 0; i < numTasks; ++i) {
		if (tasks[i]->elapsedTime == tasks[i]->period) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		GCD = findGCD(GCD,tasks[i]->period);
		tasks[i]->elapsedTime += GCD;
	}
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
