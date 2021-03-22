/*	Author: Najmeh M.A
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #12  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *      Demo Link: https://drive.google.com/file/d/1MBnfTYGa8ga4DXS4q0H7lMUkpYgWbW8I/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include <keypad.h>
#include <PWM.h>
#include <scheduler.h>
#endif

void transmit_data(unsigned short data){
	int i;
	for (i=0 ; i < 16 ; ++i){
		PORTB = 0x08;
		PORTB |= ((data >> i ) & 0x01);
		PORTB |= 0x02;
	}
	PORTB |= 0x04;
	PORTB = 0x00;
}


unsigned short f = 0x0000;
unsigned char r[3] = {0xBF , 0xDF, 0xEF};
unsigned char p[3] = {0x3C , 0x24, 0x3C};


enum Demo_States {shift};
int Demo_Tick(int state) {
// Local Variables
	static unsigned char pattern = 0x3C; // LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0xBF; // Row(s) displaying pattern.
// 0: display pattern on row
// 1: do NOT display pattern on row
// Transitions
	switch (state) {
		case shift:
			break;
		default:
			state = shift;
			break;
			}
// Actions
	switch (state) {
		case shift:
			if ((pattern == p[0]) && (row == r[0])){ pattern = p[1]; row = r[1];} 
		else if ((pattern == p[1]) && (row == r[1])){ pattern = p[2]; row = r[2];} 
			else { pattern = p[0]; row = r[0];} 
			break;
		default:
			break;
			}
	f = 0x0000;
	f = (f | pattern) << 8; 
	f = (f | row);
	transmit_data(f);
	return state;
	}


int main() {
    /* Insert DDR and PORT initializations */
    DDRD = 0xFF; PORTD = 0x00;
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    static task task1;
    
    task *tasks[] = { &task1 };
    
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    unsigned long GCD  = tasks[0]->period;
    const char start = 0;
    task1.state = start;
    task1.period = 1;
    task1.elapsedTime = task1.period;
    task1.TickFct =&Demo_Tick;


   

    TimerSet(60);
    TimerOn();
   

	

    unsigned short i;
    while (1) {
	for (i = 0; i < numTasks; ++i) {
		if (tasks[i]->elapsedTime == tasks[i]->period) {
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		GCD = findGCD(GCD,tasks[i]->period);
		tasks[i]->elapsedTime += GCD;
	}
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
