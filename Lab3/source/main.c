/*	Author: lab
 *  Partner(s) Name: NONE
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
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
    
   DDRB = 0xFE; PORTB = 0x00;
    DDRD = 0x00; PORTD = 0x00;

    unsigned char tempB = 0x00;
    unsigned char tempD = 0x00;
    unsigned char B = 0x00;
    

    while (1) {

	tempD = PIND;
	tempB = PINB;
	B = 0x00;
	
	if (((tempD << 1) | (tempB & 0x01)) >= 0x46) {
		B = 0x02;
	}
	else if (((tempD << 1) | (tempB & 0x01)) > 0x05) {
		B = 0x04;
	}
	else {
		B = 0x00;
	}
	PORTB = B;
    }
    return 0;
}
