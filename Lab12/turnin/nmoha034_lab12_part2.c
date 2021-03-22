/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #12  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *      Demo Link: https://drive.google.com/file/d/1M3FivtFZ8cS6y-QqKQnWfMjWWVBIOWZ2/view?usp=sharing
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
		PORTB = 0x08;
		PORTB |= ((data >> i ) & 0x01);
		PORTB |= 0x02;
	}
	PORTB |= 0x04;
	PORTB = 0x00;
}

unsigned short f = 0x0000;
enum GoStates {Start_Go, Init_Go, WAIT, INC_P, INC_R, DEC_P,DEC_R};

int GoTick(int state) {

    static unsigned char pattern = 0x80; // LED pattern - 0: LED off; 1: LED on
    static unsigned char row = 0x00;     // Row(s) displaying pattern.
					 // 0: display pattern on row
					 // 1: do NOT display pattern on row

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

		else if( (~PINA & 0x03) == 0x01 ){

			state = INC_P;

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
		row = 0x00;
		break;

	case Init_Go:
		row = 0x00;
		break;

	case WAIT:
		
		break;
	case INC_P:

		if( pattern > 0x01){

			 pattern = ( pattern >> 1); 
			
		}
		else{ pattern = 0x01;}

		break;

	case INC_R:

		

		break;

	case DEC_P:

		if(  pattern < 0x80){

			 pattern = ( pattern << 1);
			
		}
		else{ pattern = 0x80;}

		break;

	case DEC_R:

		break;

	
	default:

		pattern = 0x80;
		

		break;

	}
	
	f = 0x0000;
	f = (f | pattern) << 8; 
	f = (f | row);
	transmit_data(f);
	return state;
}





int main() {
    /* Insert DDR and PORT initializations */
    DDRD = 0xFF; PORTD = 0x00;
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    static task task1;
    
    task *tasks[] = { &task1 };
    
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    unsigned long GCD  = tasks[0]->period;
    const char start = 0;

    task1.state = start;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &GoTick;


   

    TimerSet(100);
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
