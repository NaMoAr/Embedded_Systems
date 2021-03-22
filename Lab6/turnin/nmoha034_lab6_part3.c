/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #6  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo Link: https://drive.google.com/file/d/1DyI0jPgv1UYAIcc5H57qDWU5Nbobq54d/view?usp=sharing
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum States {Start,INIT,  WAIT, RESET, INC, DEC} state;

unsigned char Button1 = 0x00;
unsigned char Button2 = 0x00;
unsigned char t = 0x00;

void Tick() {

switch(state) { // Transition

	case INIT:
		
		PORTB = 0x07;
		state = Start;
		

		break;

	case Start:

		if( Button1 && !Button2 ) {

			state = INC;
			

		}

		else if (!Button1 && Button2){

			state = DEC;

		}

		else if (Button1 && Button2){

			state = RESET;

		}

		else{

			state = Start;

		}


		break;

	case WAIT:

		if( (Button1 && (t < 0x0A)) || (Button2 && (t < 0x0A))) {

			state = WAIT;

		}

		else if ( Button1 && (t >= 0x0A) ){

			state = INC;

		}


		else if ( Button2 && (t >= 0x0A) ){

			state = DEC;

		}
		
		else if ( Button1 && Button2 ){

			state = RESET;

		}

		else{

			state = Start;

		}
		
		break;

	case INC:

		t = 0x00;
		state = WAIT;

		break;


	case DEC:
		
		t = 0x00;
		state = WAIT;

		break;

	
	case RESET:

		if (Button1 && Button2 ){

			state = RESET;

		}

		else{

			state = Start;

		}

		break;

	default:

		state = INIT;

		break;

	}


switch(state) { //Actions

	case INIT:

		PORTB = 0x07;

		break;

	case Start:

		break;

	case WAIT:
		++t;
	
		break;

	case INC:

		if( PORTB < 0x09){

			PORTB = PORTB + 1;
		}

		break;

	

	case DEC:

		if( PORTB > 0x00){

			PORTB = PORTB - 1;

		}

		break;

	case RESET:

		PORTB = 0x00;

		break;

	default:

		PORTB = 0x07;

		break;

	}

}


int main() {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF;PORTB = 0x00;
	
	TimerSet(100);
	TimerOn();
	state = INIT;
	

    while (1) {
	Button1 = ~PINA & 0x01;
	Button2 = ~PINA & 0x02;
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;

    }
    
}
