/*	Author: Najmeh M.A

 *  Partner(s) Name: NONE

 *	Lab Section:022

 *	Assignment: Lab #5  Exercise #2

 *	Exercise Description: [optional - include for your own benefit]

 *

 *	I acknowledge all content contained herein, excluding template or example

 *	code, is my own original work.

 */


#include <avr/io.h>

#ifdef _SIMULATE_

#include "simAVRHeader.h"

//#include "RIMS.h"

#endif

enum States {Start,INIT,  WAIT, RESET, INC, DEC} state;


void Tick() {


switch(state) { // Transition

	case INIT:
		
		PORTC = 0x07;
		state = Start;
		

		break;

	case Start:

		if( (~PINA & 0x03) == 0x01 ) {

			state = INC;
			

		}

		else if ((~PINA & 0x03) == 0x02){

			state = DEC;

		}

		else if ((~PINA & 0x03) == 0x03){

			state = RESET;

		}

		else{

			state = Start;

		}


		break;

	case WAIT:

		if( (~PINA & 0x03) == 0x01 ) {

			state = WAIT;

		}

		else if( (~PINA & 0x03) == 0x02 ){

			state = WAIT;

		}


		else if( (~PINA & 0x03) == 0x03 ){

			state = RESET;

		}

		else{

			state = Start;

		}
		
		break;

	case INC:

		state = WAIT;

		break;


	case DEC:

		state = WAIT;

		break;

	
	case RESET:

		if ((~PINA & 0x03) == 0x03){

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

		PORTC = 0x07;

		break;

	case Start:

		break;

	case WAIT:

	
		break;

	case INC:

		if( PORTC < 0x09){

			PORTC = PORTC + 1;
		}

		break;

	

	case DEC:

		if( PORTC > 0x00){

			PORTC = PORTC - 1;

		}

		break;

	case RESET:

		PORTC = 0x00;

		break;

	default:

		PORTC = 0x07;

		break;

	}

}

int main(void) {

	DDRA = 0x00; PORTA = 0xFF;

    	DDRC = 0xFF; PORTC = 0x00;

	state = INIT;

while(1) {

       	Tick();

	}
	return 1;
}
