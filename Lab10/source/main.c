/*	Author: lab
 *  Partner(s) Name: NONE
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include <keypad.h>
#include <PWM.h>
#include "stdbool.h" 
#include <scheduler.h>
#endif

bool hasChanged = false;

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

unsigned char newCom[4] = {'N', 'N' ,'N','N'};
unsigned char temp[4] = {'N', 'N' ,'N','N'};
unsigned char m = 0x00;
unsigned char h = 0x00;
unsigned char y = 0x00;
enum ChangeStates {Start_Change, Init_Change,Change,Input, Confirm, False, True, Set};
int ChangeTick(int state) {
	y = GetKeypadKey();
	switch(state) {
		case Start_Change:
			state = Init_Change;
			break;
		case Init_Change:
			if (((~PINB & 0x80) == 0x80) && (y == '*')) {
				state = Change;
			}
			else{state = Init_Change;}
			break;
		
		case Change:
			if (((~PINB & 0x80) == 0x80) && (y == '*')){
				state = Change;
			}
			else if (((~PINB & 0x80) != 0x80) && (y != '*') && (h == 4)){
				state = Confirm;
			}
			else if(((~PINB & 0x80) == 0x80) && (h < 5) && (y != '*')){ state = Input;}
			else{state = Init_Change;}
			break;
		case Input:
			state = Change;
			break;
		case Confirm:
			h = 0x00;
			if (y == 0x00 && m < 80) {
				state = Confirm;
				++m;
			}
			else if (y == temp[h]) {
				state = True;
			}
			
			else if (m >= 80){
				state = Init_Change;
			}
			else {
				state = False;
			}
			break;
		case True:
			
			if ((h == 0x03)) {
				state = Set;
				
			}
			else if (y == 0x00) {
                                ++h;
				state = Confirm;
                        }
			else {
				state = True;
				++m;
			}
                        break;
		case False:
			if (y == 0x00) {
				h = 0x00;
				state = Confirm;
			}
			else {
				state = False;
				++m;
			}
			break;
		case Set:
                        if (y == 0x00) {
				state = Init_Change;
				h = 0x00;
			}
			else {
				state = Set;
			}
			break;
		}
	switch(state) {
		case Start_Change:
			break;
		case Init_Change:
			
			break;
		case Change:
			
			break;
		case Input:
			
			temp [h] = GetKeypadKey();
			++h;
			break;
		case Confirm:
			
			break;
		case True:
			break;
		case False:
			break;
		case Set:
			
			for (int l = 0; l <= 4 ; ++l){newCom[l] = temp[l];}
			
			hasChanged = true; 
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
	if(hasChanged == false){
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
		}
	if(hasChanged == true){
		switch(state) {
		case Start_unlock:
			state = Init_unlock;
			break;
		case Init_unlock:
			if (x == newCom[i]) {
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
			
			if (i == 0x03 && ((PORTB & 0x01) == 0x00)) {
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
//unsigned char temp = 0x00;
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
    static task task4;
    task *tasks[] = { &task1 , &task2, &task3, &task4};
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

    task4.state = start;
    task4.period = 50;
    task4.elapsedTime = task4.period;
    task4.TickFct = &ChangeTick;
    

    TimerSet(50);
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
