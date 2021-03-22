/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #2  Exercise #3
3 *	Exercise Description: [optional - include for your own benefit]
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
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char cntavail = 0x00;
	unsigned char tmpA = 0x00; 
    while (1) {
	tmpA = PINA ;
	cntavail = ((tmpA & 0x08) >> 3) + ((tmpA & 0x04) >> 2) +((tmpA & 0x02) >> 1) + (tmpA & 0x01);
	if((tmpA & 0x0F) == 0x0F){
		PORTC = 0x80;
		
	}else{
		PORTC = 4 - cntavail;
	}
	
	 }
    return 0;
}
