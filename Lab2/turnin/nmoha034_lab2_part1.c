/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #2  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA= 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char tmpA = 0x00;
	 unsigned char tmpB = 0x00;
    while (1) {

	    tmpA = PINA & 0x03;

	    if(tmpA == 0x01){
		    tmpB = 0x01;
	    } else{
		    tmpB = 0x00;

	    }

	    PORTB = tmpB;
	
	 }
    return 0;
}
