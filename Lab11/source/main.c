/*	Author: Najemeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *      Demo Link: https://drive.google.com/file/d/1KDVVuyasUV6yqhPtBxIH7bb0YL9VKnGy/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif



void transmit_data(unsigned char data){
	int i;
	for (i=0 ; i < 8 ; ++i){
		PORTC = 0x08;
		PORTC |= ((data >> i ) & 0x01);
		PORTC |= 0x02;
	}
	PORTC |= 0x04;
	PORTC = 0x00;
}

enum States {Start,INIT,  WAIT, RESET, INC_P, INC_R, DEC_P,DEC_R} state;

unsigned char j = 0x00;

void Tick() {


switch(state) { // Transition

	case Start:

		state =INIT;

		break;

	case INIT:

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

		state = Start;

		break;

	}


switch(state) { //Actions

	case Start:

		j = 0x07;
		transmit_data(j);


		break;

	case INIT:

		j = 0x07;
		transmit_data(j);


		break;

	case WAIT:

		

		break;

	case INC_P:

		if( j < 0xFF){

			j = j + 1;
			transmit_data(j);
		}

		break;

	case INC_R:

		

		break;

	case DEC_P:

		if( j > 0x00){

			j = j - 1;
			transmit_data(j);

		}

		break;

	case DEC_R:

		break;

	case RESET:

		j = 0x00;
		transmit_data(j);

		break;

	default:

		j = 0x07;
		transmit_data(j);

		break;

	}

}





int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	state = Start;
    /* Insert your solution below */
    while (1) {

	//transmit_data(0xF0);
	
	Tick();
	
    }
    return 1;
}
