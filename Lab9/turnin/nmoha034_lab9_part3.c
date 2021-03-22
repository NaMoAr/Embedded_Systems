/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #9  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo Link: https://drive.google.com/file/d/1G3j-D3lAlaR3cpApfSWQS9LckMXlKQL6/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum ThreeLEDStates {ThreeLEDStart, Zero, One, Two} ThreeLEDstate;
enum BlinkingLEDStates {BlinkingLEDStart, BlinkingInit, Three} BlinkingLEDstate;
enum CombineLEDStates {CombineLEDStart, CombineInit} CombineLEDstate;
enum SpeakerStates {SpeakerStart, Off, On, Off_1} Speakerstate;
unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;
unsigned char Speaker = 0x00;
unsigned char i = 0;

void TickThreeLEDsSM() {
	switch(ThreeLEDstate) {
		case ThreeLEDStart:	ThreeLEDstate = Zero; break;
		
		case Zero:	ThreeLEDstate = One;
				break;

		case One:	ThreeLEDstate = Two;
				break;

		case Two:	ThreeLEDstate = Zero;
				break;

		default:	ThreeLEDstate = ThreeLEDStart; break;
	}

	switch (ThreeLEDstate) {
		case ThreeLEDStart:        break;
		case Zero:		threeLEDs = 0x01; break;
		case One:		threeLEDs = 0x02; break;
		case Two:		threeLEDs = 0x04; break;
                default:        	break;
	}
}

void TickBlinkingLEDSM() {
	switch (BlinkingLEDstate) {
                case BlinkingLEDStart:     BlinkingLEDstate = BlinkingInit; break;
                case BlinkingInit:         BlinkingLEDstate = Three; break;
                case Three:     	   BlinkingLEDstate = BlinkingInit; break;
                default:        	   BlinkingLEDstate = BlinkingLEDStart; break;
        }
	switch (BlinkingLEDstate) {
                case BlinkingLEDStart:     break;
                case BlinkingInit:      blinkingLED = 0x00; break;
                case Three:    		blinkingLED = 0x01; break;
                default:        	break;
        }
}



void TickSpeakerSM() {
        switch (Speakerstate) {
                case SpeakerStart:      Speakerstate = Off; break;
                case Off:        	if ((~PINA & 0x04) == 0x04) {
						Speakerstate = On;
					}
					else {
						Speakerstate = Off;
					}	
					break;
		case On:		if ((~PINA & 0x04) == 0x00) {
						Speakerstate = Off;
					}
					else if((i >= 2) & ((~PINA & 0x04) == 0x04) ) {
						Speakerstate = Off_1;
						i = 0x00;}
					else {
						Speakerstate = On;
					}
		case Off_1:		if ((~PINA & 0x04) == 0x00) {
						Speakerstate = Off;
					}
					else if((i >= 2) & ((~PINA & 0x04) == 0x04) ) {
						Speakerstate = On;
						i = 0x00;}
					else{Speakerstate = Off_1;}
					break;
                default:                Speakerstate = SpeakerStart; break;
        }
        switch (Speakerstate) {
                case SpeakerStart:      break;

                case Off:        	Speaker = 0x00; i = 0; break;

		case On:		Speaker = 0x01;
					++i;
					break;
		case Off_1:		Speaker = 0x00;
					++i;
					break;
	        default:                break;
        }
}

void TickCombineLEDsSM() {
        switch (CombineLEDstate) {
                case CombineLEDStart:      CombineLEDstate = CombineInit; break;
                case CombineInit:       CombineLEDstate = CombineInit; break;
                default:		CombineLEDstate = CombineLEDStart; break;
        }
        switch (CombineLEDstate) {
                case CombineLEDStart:      break;
                case CombineInit:       PORTB = ((Speaker << 4 ) | (blinkingLED << 3) | (threeLEDs)); break;
                default:       		break;
        }
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    unsigned long ThreeLEDElapsed = 0;
    unsigned long BlinkingLEDElapsed = 0;
    const unsigned long timerPeriod = 1;
    TimerSet(1);
    TimerOn();
    ThreeLEDstate = ThreeLEDStart;
    BlinkingLEDstate = BlinkingLEDStart;
    CombineLEDstate = CombineLEDStart;
    Speakerstate = SpeakerStart;

    while (1) {
	if (ThreeLEDElapsed >= 300) {
		TickThreeLEDsSM();
		ThreeLEDElapsed = 0;
	}
	if (BlinkingLEDElapsed >= 1000) {
		TickBlinkingLEDSM();
		BlinkingLEDElapsed = 0;
	}
	TickSpeakerSM();
	TickCombineLEDsSM();
	while(!TimerFlag) {};
	TimerFlag = 0;
	ThreeLEDElapsed += timerPeriod;
	BlinkingLEDElapsed += timerPeriod;
    }
    return 0;
}
