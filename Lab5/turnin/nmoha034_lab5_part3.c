/* Author: Najmeh M.A
 * Partner(s) Name (if applicable):  
 * Lab Section:022
 * Assignment: Lab #5  Exercise #3
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 *
 *  Demo Link: https://drive.google.com/file/d/1t1kIgxExae9LYvc7J4Teq1gnRrPao6Sy/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned array[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F};
unsigned char i = 0x00;
unsigned char Change_Direction = 0x00;
enum States {Start, Begin, Init, Reset, ResetOn, INC, DEC, INC_ON, DEC_ON} state;
void Tick() {
	switch(state) {
		case Start:
			state = Begin;
			break;
		case Begin:
			state = Init;
			break;
		case Init:
			if ((i > 0x07) || (i < 0x00)) {
				state = ResetOn;
			}
			else if ((~PINA & 0x01) == 0x01) {
				if ((Change_Direction & 0x01) == 0x01) {
                                	state = DEC_ON;
                        	}
				else {
					state = INC_ON;
				}
			}
			else {
                        	state = Init;
                        }
			break;
		case Reset:
			if ((~PINA & 0x01) == 0x01) {
                                state = Reset;
                        }
                        else {
                                state = Begin;
                        }
                        break;
		case ResetOn:
			state = Reset;
			break;
		case INC:
			if ((~PINA & 0x01) == 0x01) {
                                state = INC;
                        }
                        else {
                                state = Init;
                        }
                        break;
		case INC_ON:
			state = INC;
			break;
			
		case DEC:
                        if ((~PINA & 0x01) == 0x01) {
                                state = DEC;
                        }
                        else {
                                state = Init;
                        }
                        break;
		case DEC_ON:
			state = DEC;
			break;
			
		default:
			state = Start;
			break;
	}
	switch(state) {
		case Start:
			PORTB = 0x00;
			break;
		case Begin:
			PORTB = 0x00;
			break;
		case Init:
			break;
		case INC:
			break;
		case DEC:
			break;
		case INC_ON:
			
			PORTB = array[i];
			++i;
                        break;
		case DEC_ON:
			
			PORTB = array[i];
			--i;
                        break;
		case Reset:
			break;
		case ResetOn:
			PORTB = 0x00;
			if ((Change_Direction & 0x01) == 0x01) {
				Change_Direction = 0x00;
				i = 0x00;
			}
			else {
				Change_Direction = 0x01;
				i = 0x06;
			}
			break;
		default:
			PORTB = 0x00;
			break;
	}
}

int main(void) {
    
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    while (1) {
	Tick();
	
    }
    return 1;
}
