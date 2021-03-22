/*	Author:Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #10  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *      Demo Link: https://drive.google.com/file/d/1KHfGynUccThpw9zIY-NZ_F__JgIDdPRs/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include <keypad.h>
#include <PWM.h>
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
unsigned char keypad[6] = {'#', '1' ,'2','3', '4' ,'5'};
enum UnlockStates {Start_unlock, Init_unlock, Correct, Wrong, Unlock};
int UnlockTick(int state) {
	x = GetKeypadKey();
	switch(state) {
		case Start_unlock:
			state = Init_unlock;
			break;
		case Init_unlock:
			if (x == keypad[i]) {
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




enum States {Start_bell, Init_bell, MUSIC, RELEASE};
unsigned char j = 0x00;
unsigned char temp = 0x00;
double array[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

int BellTick(int state) {
	switch(state) {
		case Start_bell:	state = Init_bell; break;
		
		case Init_bell:	if ((~PINA & 0x80) == 0x80) {state = MUSIC;}
				else {state = Init_bell;}
				break;

		case MUSIC:	if( j >= 15 ) {state = RELEASE;}
				else { state = MUSIC;}
				break;

		case RELEASE:	if ((~PINA & 0x80) == 0x80) {state = RELEASE;}
                                else {state = Init_bell;}
                                break;

		//default:	state = Start_bell; break;
	}
	switch (state) {
		case Start_bell:     break;
		case Init_bell:	set_PWM(0); j = 0; break;
		
		case MUSIC:	if (j < 2) {
					set_PWM(array[0]);
				}
				else if (j < 3) {
					set_PWM(array[2]);
                                }
				else if (j < 6) {
					set_PWM(array[4]);
                                }
				else if (j < 7) {
                                        set_PWM(array[1]);
                                }
				else if (j < 9) {
					set_PWM(array[3]);
                                }
				else if (j < 12) {
                                        set_PWM(array[5]);
                                }
				else if (j < 13) {
					set_PWM(array[8]);
                                }
				else if (j < 15) {
					set_PWM(array[6]);
                                }
				++j;
				break;
		case RELEASE:	set_PWM(0); break;
                //default:        break;
	}
	return state;
}





int main() {
    /* Insert DDR and PORT initializations */
    DDRB = 0x7F; PORTB = 0x00;
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xF0; PORTC = 0x0F;
    /* Insert your solution below */
    static task task1;
    static task task2;
    static task task3;
    task *tasks[] = { &task1 , &task2, &task3};
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

    task3.state = start;
    task3.period = 200;
    task3.elapsedTime = task3.period;
    task3.TickFct = &BellTick;
    

    TimerSet(50);
    TimerOn();
    TimerOn();
    PWM_on();

	

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
