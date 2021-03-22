/*	Author: Najmejeh
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #11  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *      Demo Link: https://drive.google.com/file/d/1KNeCZi4zUDtczsz39PhqKrHS9_mX-uZx/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include <keypad.h>
#include <PWM.h>
#include <scheduler.h>
#endif

void transmit_data(unsigned char data){
	int t;
	for (t=0 ; t < 8 ; ++t){
		PORTC = 0x08;
		PORTC |= ((data >> t ) & 0x01);
		PORTC |= 0x02;
	}
	PORTC |= 0x04;
	PORTC = 0x00;
}

unsigned char go = 0x00;

enum GoStates {Start_Go, Init_Go, WAIT, RESET, INC_P, INC_R, DEC_P,DEC_R};

int GoTick(int state) {

    switch(state) { // Transition

	case Start_Go:

		state =Init_Go;

		break;

	case Init_Go:

		state = WAIT;

		break;

	case WAIT:

		if( (~PINA & 0x03) == 0x01 ) {

			state = INC_P;

		}

		else if ((~PINA & 0x03) == 0x02){

			state = DEC_P;

		}

		else if ((~PINA & 0x03) == 0x03){

			state = RESET;

		}

		else{

			state = WAIT;

		}

		break;

	case INC_P:

		state = INC_R;

		break;

	case INC_R:

		if( (~PINA & 0x03) == 0x01 ) {

			state = INC_R;

		}

		else if( (~PINA & 0x03) == 0x03 ){

			state = RESET;

		}

		else if( (~PINA & 0x03) == 0x02 ){

			state = DEC_P;

		}

		else{

			state = WAIT;

		}

		break;

	case DEC_P:

		state = DEC_R;

		break;

	case DEC_R:

		if( (~PINA & 0x03) == 0x02 ) {

			state = DEC_R;

		}

		else if( (~PINA & 0x03) == 0x03 ){

			state = RESET;

		}

		else if( (~PINA & 0x03) == 0x01 ){

			state = INC_P;

		}

		else{

			state = WAIT;

		}

		break;

	case RESET:

		if ((~PINA & 0x03) == 0x03){

			state = RESET;

		}

		else{

			state = WAIT;

		}

		break;

	default:

		state = Start_Go;

		break;

	}


   switch(state) { //Actions

	case Start_Go:
		go = 0x01;
		break;

	case Init_Go:
		go = 0x01;
		break;

	case WAIT:
		
		break;
	case INC_P:

		if( go < 0x03){

			go = go + 1;
			
		}

		break;

	case INC_R:

		

		break;

	case DEC_P:

		if( go > 0x01){

			go = go - 1;
			

		}

		break;

	case DEC_R:

		break;

	case RESET:

		if(go == 0x00){go = 0x01;}
		else {go = 0x00;
		      transmit_data(0x00);
			}
		

		break;

	default:

		go = 0x01;
		

		break;

	}

	return state;
}




unsigned char j = 0x00;
unsigned char Direction = 0x00;
unsigned char LED = 0x00;

enum festiveOneStates {Start_One, Init_One, One};
int festiveOneTick(int state) {
if (go == 0x01){
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
			j = 0x00;
			transmit_data(j);
			break;
		case Init_One:
			j = 0x01;
			transmit_data(j);
			break;
		case One:
			if(Direction == 0x00){
					if(j == 0x80){
						
						LED  = (j >> 1);
						j = LED;
						transmit_data(j);
						Direction = 0x01;
					}
					else{
						LED  = (j << 1);
						j = LED;
						transmit_data(j);
						
					}
				}
				else{
					if (j == 0x01) {
                                        LED  = j << 1;
					j = LED ;
					transmit_data(j);
                                        Direction = 0x00;
                               		}
                                	else {
                                        LED = j >> 1;
					j = LED;
					transmit_data(j);
                                	}

				}
                        break;
		}
		
}
	return state;
}


enum festivtTwoStates {Start_Two, Init_Two, Two};
int festivtTwoTick(int state) {
if(go == 0x02){
	switch(state) {
		case Start_Two:
			state = Init_Two;
			break;
		case Init_Two:
			state = Two;
			break;
		case Two:
			state =Two;
			
                        break;
		}
	switch(state) {
		case Start_Two:
			j = 0x00;
			transmit_data(j);
			break;
		case Init_Two:
			j = 0x81;
			transmit_data(j);
			break;
		case Two:
			if(Direction == 0x00){
					if(j == 0x81){
						j = 0xC3;
						transmit_data(j);
						
					}
					else if (j == 0xC3){
						j = 0xE7;
						transmit_data(j);
						
					}
					else{
						j = 0xFF;
						transmit_data(j);
						Direction = 0x01;
					}
			}
			else{
					if(j == 0xC3){
						j = 0x81;
						transmit_data(j);
						Direction = 0x00;
						
					}
					else if (j ==  0xE7){
						j = 0xC3;
						transmit_data(j);
						
					}
					else{
						j = 0xE7;
						transmit_data(j);
						
					}

				}
                        break;
		}
		
}	
	return state;
}



enum festivtThreeStates {Start_Three, Init_Three, Three};

int festivtThreeTick(int state) {
if (go == 0x03){
	switch(state) {
		case Start_Three:
			state = Init_Three;
			break;
		case Init_Three:
			state = Three;
			break;
		case Three:
			state =Three;
			
                        break;
		}
	switch(state) {
		case Start_Three:
			j = 0x00;
			transmit_data(j);
			break;
		case Init_Three:
			j = 0x81;
			transmit_data(j);
			break;
		case Three:
			
					if(j == 0x81){
						j = 0x44;
						transmit_data(j);
						
					}
					else if(j == 0x44){
						j = 0xC3;
						transmit_data(j);
						
					}
					else if (j == 0xC3){
						j = 0x24;
						transmit_data(j);
						
					}
					else if (j == 0x24){
						j = 0x66;
						j = 0xE7;
						transmit_data(j);
						
					}
					else if (j == 0x66){
						j = 0xE7;
						transmit_data(j);
						
					}
					else if (j == 0xE7){
						j = 0x18;
						transmit_data(j);
						
					}
					else if (j == 0x18){
						j = 0x99;
						transmit_data(j);
						
					}
					else if (j == 0x99){
						j = 0xDB;
						transmit_data(j);
						
					}
					else if (j == 0xDB){
						j = 0xFF;
						transmit_data(j);
					}
					else {
						j = 0x81;
						transmit_data(j);
                                        }
			
                        break;
		}
		
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
    static task task4;
    task *tasks[] = { &task1 , &task2, &task3, &task4};
    
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    unsigned long GCD  = tasks[0]->period;
    const char start = 0;

    task1.state = start;
    task1.period = 500;
    task1.elapsedTime = task3.period;
    task1.TickFct = &GoTick;

    task2.state = start;
    task2.period = 500;
    task2.elapsedTime = task1.period;
    task2.TickFct = &festiveOneTick;
   

    task3.state = start;
    task3.period = 500;
    task3.elapsedTime = task2.period;
    task3.TickFct = &festivtTwoTick;

    task4.state = start;
    task4.period = 500;
    task4.elapsedTime = task3.period;
    task4.TickFct = &festivtThreeTick;
   

    TimerSet(500);
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
