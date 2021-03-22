/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #8  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo Link: https://drive.google.com/file/d/1Gi0QDGWytHLkBy0CmSMJWv7p41Wh55ui/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif



void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		else { OCR3A = (short) (8000000 / (128 * frequency)) -1; }
		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum States {Start, Init, MUSIC, RELEASE} state;
unsigned char i = 0x00;
unsigned char temp = 0x00;
double array[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

void Tick() {
	switch(state) {
		case Start:	state = Init; break;
		
		case Init:	if ((~PINA & 0x01) == 0x01) {state = MUSIC;}
				else {state = Init;}
				break;

		case MUSIC:	if( i >= 100 ) {state = RELEASE;}
				else { state = MUSIC;}
				break;

		case RELEASE:	if ((~PINA & 0x01) == 0x01) {state = RELEASE;}
                                else {state = Init;}
                                break;

		default:	state = Start; break;
	}
	switch (state) {
		case Start:     break;
		case Init:	set_PWM(0); i = 0; break;
		
		case MUSIC:	if (i < 5) {
					set_PWM(array[0]);
				}
				else if (i < 10) {
					set_PWM(array[2]);
                                }
				else if (i < 13) {
					set_PWM(array[4]);
                                }
				else if (i < 20) {
                                        set_PWM(array[1]);
                                }
				else if (i < 23) {
					set_PWM(array[3]);
                                }
				else if (i < 24) {
                                        set_PWM(array[5]);
                                }
				else if (i < 27) {
					set_PWM(array[8]);
                                }
				else if (i < 29) {
					set_PWM(array[6]);
                                }
				else if (i < 30) {
					set_PWM(array[7]);
                                }
				else if (i < 35) {
                                        set_PWM(array[2]);
                                }
				else if (i < 40) {
					set_PWM(array[1]);
                                }
				else if (i < 45) {
					set_PWM(array[8]);
				}
				else if (i < 49) {
					set_PWM(array[7]);
                                }
				else if (i < 52) {
					set_PWM(array[3]);
				}
				else if (i < 63) {
					set_PWM(array[4]);
                                }
				else if (i < 76) {
					set_PWM(array[2]);
				}
				else if (i < 85) {
					set_PWM(array[0]);
                                }
				else if (i < 95) {
					set_PWM(array[1]);
				}
				++i;
				break;
		case RELEASE:	set_PWM(0); break;
                default:        break;
	}
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(50);
    TimerOn();
    PWM_on();
    while (1) {
	Tick();
	while(!TimerFlag) {};
	TimerFlag = 0;
    }
	return 0;
}
