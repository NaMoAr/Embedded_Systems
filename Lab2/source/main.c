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
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA= 0x00;
	DDRB = 0x00; PORTB= 0x00;
	DDRC = 0x00; PORTC= 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	//DDRC = 0xFF; PORTC = 0x00;
	/*while(1){
	PORTC = (PINA & 0x80 >> 7) + (PINA & 0x40 >> 6) + (PINA & 0x20 >> 5) + (PINA & 0x10>> 4) + (PINA & 0x08 >> 3) + (PINA & 0x04 >> 3) + (PINA & 0x02 >> 1) + (PINA &0x1) +  (PINB & 0x80 >> 7) + (PINB & 0x40 >> 6) + (PINB & 0x20 >> 5) + (PINB & 0x10>> 4) + (PINB & 0x08 >> 3) + (PINB & 0x04 >> 3) + (PINB & 0x02 >> 1) + (PINB &0x1);  

	}*/
	//unsigned char cntavail = 0x00;
	unsigned char tmpD = 0x00; 
	unsigned char tmpA = 0x00;
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;


   while (1) {
	tmpA = PINA;
	tmpB = PINB;
	tmpC = PINC;
if((tmpA + tmpB + tmpC) >  0x8C){
                tmpD = 0x01;
        }
	else{
		tmpD = 0x00;
	}
	if((tmpA - tmpC  > 0x50) || (tmpC - tmpA > 0x50)){
		tmpD = tmpD | 0x02;
	}

        PORTD = tmpD | (((tmpA + tmpB + tmpC) >> 2) & 0xFC);
         }

    return 0;
}
