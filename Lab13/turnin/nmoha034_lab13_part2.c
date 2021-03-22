/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #13  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *      Demo Link: https://drive.google.com/file/d/1NOKrVCkg2ZLzDHY7tVhdfKRrQ7kvB-mZ/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include <keypad.h>
#include <PWM.h>
#include <scheduler.h>
#endif


void A2D_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1<< ADATE);
}

unsigned short input = 0x0000;


enum GoStates {Start_Go, Init_Go, WAIT, LEFT, RIGHT};

int GoTick(int state) {

    static unsigned char pattern = 0x80; // LED pattern - 0: LED off; 1: LED on
    static unsigned char row = 0xFE;     // Row(s) displaying pattern.
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

		if (input > 543 ){state= RIGHT;}
		else if (input < 543){state = LEFT;}
		else{state = WAIT;}
		

		break;
	case LEFT:
		state = WAIT;
		break;
	case RIGHT:
		state = WAIT;
		break;


	default:

		state = Start_Go;

		break;

	}


   switch(state) { //Actions

	case Start_Go:
		pattern = 0x80;
		row = 0xFE;
		break;

	case Init_Go:
		pattern = 0x80;
		row = 0xFE;
		break;

	case WAIT:
		
		break;
	
	case LEFT:
		if (pattern < 0x80){
		pattern = (pattern << 1) ; }	
		else{pattern = 0x01;}
		break;
	case RIGHT:
		if(pattern > 0x01){
		pattern = (pattern >> 1) ; }
		else{pattern = 0x80;}
		break;
	default:
		
		pattern = 0x80;
		row = 0xFE;

		break;

	}
	
	PORTC = pattern; // Pattern to display
	PORTD = row; // Row(s) displaying pattern
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
    task1.period = 50;
    task1.elapsedTime = task1.period;
    task1.TickFct = &GoTick;


   

    TimerSet(50);
    TimerOn();
    A2D_init();

	

    unsigned short i;
    while (1) {
	
	input = ADC;
	
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




