/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo Link: https://drive.google.com/file/d/1E-ID2LJM9qzhZCV1rB66o5l_gx0_dacW/view?usp=sharing
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum States {Start, Init, LED_Seq,Pause, Press_1, Press_2} state;
unsigned char Button = 0x00;
unsigned char Direction = 0x00;
unsigned char LED = 0x00;

void Tick() {
	
	switch(state) {
		case Start:	state = Init; break;
		case Init:	state = LED_Seq; break;
		case LED_Seq:	if (!Button) {state = LED_Seq;} else{state = Pause;} break;
		case Pause:	if (Button) {state = Pause;}
				else {state = Press_1;}
				break;
		case Press_1:	if (Button) {state = Press_2;} else{state = Press_1;} break;
		case Press_2:	if (Button) {state = Press_2;} else{state = Init;} break;
		default:	state = Start; break;
	}
	switch (state) {
		case Start:	break;
		case Init:	PORTB = 0x01; break;
		case LED_Seq:	if(Direction == 0x00){
					if(PORTB == 0x04){
						LED = PORTB >> 1;
						PORTB = LED;
						Direction = 0x01;
					}
					else{
						LED = PORTB << 1;
						PORTB = LED;
					}
				}
				else{
					if (PORTB == 0x01) {
                                        PORTB = PORTB << 1;
                                        Direction = 0x00;
                               		}
                                	else {
                                        PORTB = PORTB >> 1;
                                	}

				}
				break;
		case Pause: 	break;
		case Press_1:	break;
		case Press_2:	break;
		default:	PORTB = 0x01; break;
	}
}
int main() {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF;PORTB = 0x00;
	
	TimerSet(300);
	TimerOn();
	
	

    while (1) {
	Button = ~PINA & 0x01;
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;

    }
    
}
