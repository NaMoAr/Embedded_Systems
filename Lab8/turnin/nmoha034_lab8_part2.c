/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #8  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	Demo Link: https://drive.google.com/file/d/1GYuD76a5UyQN-pnUmi9FmnFbAq-wAelk/view?usp=sharing
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

enum States {Start, Init, INC, DEC, ONOFF, RELEASE} state;
unsigned char on = 0x00;
unsigned char temp = 0x00;
double array[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

void Tick() {
	switch(state) {
		case Start:	state = Init; break;
		
		case Init:	if ((~PINA & 0x07) == 0x01) {state = INC;}
				else if ((~PINA & 0x07) == 0x02) {state = DEC;}
				else if ((~PINA & 0x07) == 0x04) {state = ONOFF;}
				else {state = Init;}
				break;

		case INC:	state = RELEASE;
				break;

		case DEC:	state = RELEASE;
                                break;

		case ONOFF:	state = RELEASE;
				break;

		case RELEASE:	if ((~PINA & 0x07) == 0x00) {state = Init;}
                                else {state = RELEASE;}
                                break;

		default:	state = Start; break;
	}
	switch (state) {
		case Start:     break;
		case Init:	break;
		case INC:	if (temp < 0x07) {++temp;} 
				if (on == 0x01) {set_PWM(array[temp]);}
				break;
		case DEC:	if (temp > 0x00) {--temp;} 
				if (on ==0x01) {set_PWM(array[temp]);}
				break;
		case ONOFF:	if (on == 0x00) {on = 0x01; set_PWM(array[temp]);}
				else {on = 0x00; set_PWM(0);} 
				break;
		case RELEASE:	break;
                default:        break;
	}
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(100);
    TimerOn();
    PWM_on();
    while (1) {
	Tick();
	while(!TimerFlag) {};
	TimerFlag = 0;
    }
	return 0;
}
