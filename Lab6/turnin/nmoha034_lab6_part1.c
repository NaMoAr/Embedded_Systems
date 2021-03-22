/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *      Demo Link: https://drive.google.com/file/d/1E-6sjTrVbPYLoNruyn0CtlPMz2SuXUWZ/view?usp=sharing
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum States {Start, Init, LED_Seq} state;

unsigned char LED = 0x00;

void Tick() {
	
	switch(state) {
		case Start:	state = Init; break;
		case Init:	state = LED_Seq; break;
		case LED_Seq:	state = LED_Seq; break;
		default:	state = Start; break;
	}
	switch (state) {
		case Start:	break;
		case Init:	PORTB = 0x01; break;
		case LED_Seq:	if(PORTB == 0x04){
					PORTB = 0x01;	
				}
				else{
					LED = PORTB << 1;
					PORTB = LED;
				}
				break;
		default:	PORTB = 0x01; break;
	}
}
int main() {
	
	DDRB = 0xFF;PORTB = 0x00;
	
	TimerSet(1000);
	TimerOn();
	

    while (1) {
	
	Tick();
	while(!TimerFlag);
	TimerFlag = 0;

    }
    
}
