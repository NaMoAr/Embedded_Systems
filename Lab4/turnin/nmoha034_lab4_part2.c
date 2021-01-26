/*	Author: Najmeh M.A

 *  Partner(s) Name: NONE

 *	Lab Section:022

 *	Assignment: Lab #4  Exercise #2

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

enum States {Start,INIT,  WAIT, RESET, INC_P, INC_R, DEC_P,DEC_R} state;


void Tick() {


switch(state) { // Transition

	case Start:

		state =INIT;

		break;

	case INIT:

		state = WAIT;

		break;

	case WAIT:

		if( (PINA & 0x03) == 0x01 ) {

			state = INC_P;

		}

		else if ((PINA & 0x03) == 0x02){

			state = DEC_P;

		}

		else if ((PINA & 0x03) == 0x03){

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

		if( (PINA & 0x03) == 0x01 ) {

			state = INC_R;

		}

		else if( (PINA & 0x03) == 0x03 ){

			state = RESET;

		}

		else if( (PINA & 0x03) == 0x02 ){

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

		if( (PINA & 0x03) == 0x02 ) {

			state = DEC_R;

		}

		else if( (PINA & 0x03) == 0x03 ){

			state = RESET;

		}

		else if( (PINA & 0x03) == 0x01 ){

			state = INC_P;

		}

		else{

			state = WAIT;

		}

		break;

	case RESET:

		if ((PINA & 0x03) == 0x03){

			state = RESET;

		}

		else{

			state = WAIT;

		}

		break;

	default:

		state = Start;

		break;

	}


switch(state) { //Actions

	case Start:

		PORTC = 0x07;

		break;

	case INIT:

		PORTC = 0x07;

		break;

	case WAIT:

		

		break;

	case INC_P:

		if( PORTC < 0x09){

			PORTC = PORTC + 1;

		}

		break;

	case INC_R:

		

		break;

	case DEC_P:

		if( PORTC > 0x00){

			PORTC = PORTC - 1;

		}

		break;

	case DEC_R:

		break;

	case RESET:

		PORTC = 0x00;

		break;

	default:

		PORTC = 0x07;

		break;

	}

}

int main() {

	DDRA = 0x00; PORTA = 0x00;

    	DDRC = 0xFF; PORTC = 0x00;

	state = Start;

while(1) {

       	Tick();

	}
}
