/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #7  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo Link: https://drive.google.com/file/d/1DkvZUdY4B-jz4-ldqxGNtgS89hfmaV4P/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
unsigned short my_short = 0xABCD;
unsigned short max = 0xFF;

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1<< ADATE);
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    ADC_init();
    while (1) {
	my_short = ADC;
	unsigned char my_short_char = (char)my_short;
	
	if (my_short_char >= max/2) {
		PORTB = 0x01;
	}
	else {
		PORTB = 0x00;
	}
    }
    return 0;
}
