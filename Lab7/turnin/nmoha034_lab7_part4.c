/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #7  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo Link: https://drive.google.com/file/d/1DeHG2Oryxon2GEIiat5CSwXQDPAQ96vy/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
unsigned short my_short = 0x00;
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
	unsigned coeff = max/8;
	
	if (my_short_char <= (coeff * 3.5)) {
		PORTB = 0x01;
	}
	else if (my_short_char <= (coeff * 3.75)) {
                PORTB = 0x03;
        }
	else if (my_short_char <= (coeff * 3.90)) {
		PORTB = 0x07;
	}
	else if (my_short_char <= (coeff * 4)) {
                PORTB = 0x0F;
        }
	else if (my_short_char <= (coeff * 5)) {
                PORTB = 0x1F;
        }
	else if (my_short_char <= (coeff * 6)) {
                PORTB = 0x3F;
        }
	else if (my_short_char <= (coeff * 7)) {
                PORTB = 0x7F;
        }
	else if (my_short_char <= (coeff * 8)) {
		PORTB = 0xFF;
	}
    }
    return 0;
}
