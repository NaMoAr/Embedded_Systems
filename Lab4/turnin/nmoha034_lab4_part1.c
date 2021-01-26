/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #4  Exercise #1
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
enum States {Start, OFF_RELEASE, ON_PRESS, ON_RELEASE, OFF_PRESS} state;

void Tick() {

switch(state) {
	case Start:
		PORTB = 0x01;
		state = OFF_RELEASE;
		break;
	case OFF_RELEASE:
		if( (PINA & 0x01) == 0x01 ) {
			PORTB = 0x02;
			state = ON_PRESS;
		}
		else{
			state = OFF_RELEASE;
		}
		break;
	case ON_PRESS:
		state = ((PINA & 0x01) == 0x01) ? ON_PRESS : ON_RELEASE;
		break;
	case ON_RELEASE:
		if( (PINA & 0x01) == 0x01 ) {
			PORTB = 0x01;
			state = OFF_PRESS;
		}
		else{
			state = ON_RELEASE;
		}
		break;
	case OFF_PRESS:
		state = ((PINA & 0x01) == 0x01) ? OFF_PRESS : OFF_RELEASE;
		break;
	default:
		break;
	}

}
void main() {
	DDRA = 0x00; PORTA = 0xFF;
    	DDRB = 0xFF; PORTB = 0x00;
	state = Start;
while(1) {
       	Tick();
	}
}
