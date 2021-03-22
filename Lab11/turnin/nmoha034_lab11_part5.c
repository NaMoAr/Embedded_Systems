/*	Author: Najmeh
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #11  Exercise #5
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *      Demo Link: https://drive.google.com/file/d/1Kc6b74jvIXdiFL9DlU-kwR-Z9dyKNJ5B/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include <keypad.h>
#include <PWM.h>
#include <scheduler.h>
#endif


void transmit_data(unsigned short data){
	int i;
	for (i=0 ; i < 16 ; ++i){
		PORTC = 0x08;
		PORTC |= ((data >> i ) & 0x01);
		PORTC |= 0x02;
	}
	PORTC |= 0x04;
	PORTC = 0x00;
}
unsigned short j = 0x00;
unsigned char Direction = 0x00;
unsigned short LED = 0x0000;
unsigned char right = 0x00;
unsigned char jump = 0x00;
unsigned char left = 0x00;
unsigned char dir = 0x00;
unsigned short k = 0x0000;
unsigned char m = 0x00;

enum collisionStates {Start_collision, Init_collision, GameOver};

int collisionTick(int state) {

    switch(state) { // Transition
		case Start_collision:
			state = Init_collision;
		break;
		case Init_collision:
			if ( k == j ){state = GameOver;}
		break;
		case GameOver:
			state = GameOver;
		break;
	}


   switch(state) { //Actions

	

	case Start_collision:
		break;
		case Init_collision:
		break;
		case GameOver:
			if (m%2 == 0){
				transmit_data(0x0000);
			}
			else{transmit_data(0xFFFF);}
			++m;
		break;

	}

	return state;
}

enum playerStates {Start, Init, Wait, Left, Right, Jump_r, Jump_l, Rel};
int playerTick(int state) {
	
	switch(state) {
		case Start:
			state = Init;
			break;
		case Init:
			state = Wait;
			break;
		case Wait:
			if ((~PINA & 0x07) == 0x01){state = Right;}
			else if ((~PINA & 0x07) == 0x04){state = Left;}

			else if ( ((~PINA & 0x07) == 0x03) ){state = Jump_r;}
			else if ( ((~PINA & 0x07) == 0x06) ){state = Jump_l;}
			else{state = Wait;}
                        break;
		case Right:
			state = Rel;
			break;
		case Jump_l:
			
			state = Rel;
			break;
		case Jump_r:
			
			state = Rel;
			break;
		case Left:
			
			state = Rel;
			break;
		case Rel:
			if ((~PINA & 0x07) == 0x00){ 
			state = Wait;}
			else{state = Rel;}
			break;
		}
	switch(state) {
		case Start:
			k = 0x0080;
			transmit_data(k| j);
			break;
		case Init:
			k = 0x0080;
			transmit_data(k| j);
			break;
		case Wait:
                        break;
		case Right:
			if ( k != 0x0001){
			k = k >> 1;
			transmit_data(k | j);}
			else{transmit_data(0x0001 | j); }
			break;
		case Jump_r:
			if (k > 0x0004){
			k = k >> 4;transmit_data(k| j);}
			else{transmit_data(0x0001 | j); }
			break;
		case Jump_l:
			if  (k < 0x2000){
			k = k << 4;transmit_data(k| j);}
			else{transmit_data(0x8000 | j);}
			break;
		case Left:
			if (k != 0x8000){
			k = k << 1;
			transmit_data(k | j);}
			else{
			transmit_data(0x8000 | j); 
			}
			break;
		case Rel:
			break;
		}
		
	
	return state;
}








enum enemyStates {Start_One, Init_One, One};
int enemyTick(int state) {

	switch(state) {
		case Start_One:
			state = Init_One;
			break;
		case Init_One:
			state = One;
			break;
		case One:
			state = One;
			
                        break;
		}
	switch(state) {
		case Start_One:
			j = 0x0001;
			transmit_data(j | k );
			break;
		case Init_One:
			j = 0x0001;
			transmit_data(j | k);
			break;
		case One:
			if(Direction == 0x00){
					if(j == 0x8000){
						
						LED  = (j >> 1);
						j = LED;
						transmit_data(j | k );
						Direction = 0x01;
					}
					else{
						LED  = (j << 1);
						j = LED;
						transmit_data(j | k );
						
					}
				}
				else{
					if (j == 0x0001) {
                                        LED  = j << 1;
					j = LED ;
					transmit_data(j | k );
                                        Direction = 0x00;
                               		}
                                	else {
                                        LED = j >> 1;
					j = LED;
					transmit_data(j | k);
                                	}

				}
                        break;
		}
		

	return state;
}




int main() {
    /* Insert DDR and PORT initializations */
    DDRB = 0x7F; PORTB = 0x00;
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
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
    task1.TickFct = &playerTick;

    task2.state = start;
    task2.period = 500;
    task2.elapsedTime = task2.period;
    task2.TickFct = &enemyTick;

    task3.state = start;
    task3.period = 100;
    task3.elapsedTime = task3.period;
    task3.TickFct = &collisionTick;
  

    
   

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
