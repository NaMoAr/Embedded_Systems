/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #4  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start,LOCK, UNLOCK_1, UNLOCK_2,UNLOCK_3, UNLOCK, LOCK_1, LOCK_2, LOCK_3} state;


void Tick() {


switch(state) { // Transition

	case Start:
		state =LOCK;
		break;
	case LOCK:
		if( (PINA & 0x07) == 0x04 ) {
			state = UNLOCK_1;
		}
		else{
			state = LOCK;
		}
		break;
	case UNLOCK_1:
		if( (PINA & 0x07) == 0x00 ){
			state = UNLOCK_2;
		}
		else{
		state = UNLOCK_1;
		}
		break;
	case UNLOCK_2:
		if( (PINA & 0x07) == 0x02 ) {
			state = UNLOCK_3;
		}
		else if( (PINA & 0x087) == 0x00 ) {
			state = UNLOCK_2;
		}
		else{
			state = LOCK;
		}
		break;
	case UNLOCK_3:
		if( (PINA & 0x07) == 0x02 ) {
			state = UNLOCK_3;
		}
		else{
			state = UNLOCK;
		}
		break;
	case UNLOCK:
		if( (PINA & 0x87) == 0x80 ) {
			state = LOCK;
		}
		else if( (PINA & 0x87) == 0x04 ) {
			state = LOCK_1;
		}
		else{
			state = UNLOCK;
		}
		break;
	case LOCK_1:
		if( (PINA & 0x87) == 0x00 ){
			state = LOCK_2;
		}
		else if( (PINA & 0x87) == 0x80 ) {
			state = LOCK;
		}
		else{
		state = LOCK_1;
		}
		break;
	case LOCK_2:
		if( (PINA & 0x87) == 0x02 ) {
			state = LOCK_3;
		}
		else if( (PINA & 0x87) == 0x00 ) {
			state = LOCK_2;
		}
		else if( (PINA & 0x87) == 0x80 ) {
			state = LOCK;
		}
		else{
			state = UNLOCK;
		}
		break;
	case LOCK_3:
		if( (PINA & 0x87) == 0x02 ) {
			state = LOCK_3;
		}
		else if( (PINA & 0x87) == 0x80 ) {
			state = LOCK;
		}
		else{
			state = LOCK;
		}
		break;
	default:
		state = Start;
		break;
	}


switch(state) { //Actions

	case Start:
		PORTB = 0x00;
		break;
	case LOCK:
		PORTB = 0x00;
		break;
	case UNLOCK_1:
		PORTB = 0x00;
		break;
	case UNLOCK_2:
		PORTB = 0x00;
		break;
	case UNLOCK_3:
		PORTB = 0x00;
		break;
	case UNLOCK:
		PORTB = 0x01;
		break;
	
	case LOCK_1:
		PORTB = 0x01;
		break;
	case LOCK_2:
		PORTB = 0x01;
		break;
	case LOCK_3:
		PORTB = 0x01;
		break;
	default:
		PORTB = 0x00;
		break;
	

	}

}

int main() {

	DDRA = 0x00; PORTA = 0x00;

    	DDRB = 0xFF; PORTB = 0x00;

	state = Start;

while(1) {

       	Tick();

	}
}
