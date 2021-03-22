/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #2  Exercise #2
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
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char cntavail = 0x00;
	unsigned char tempA = 0x00;
    while (1) {
	tempA = PINA;
	cntavail = ((tempA & 0x08) >> 3) + ((tempA & 0x04) >> 2) +((tempA & 0x02) >> 1) + (tempA & 0x01);
	PORTC = 4 - cntavail;
	 }
    return 0;
}
