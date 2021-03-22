/*	Author: Najmejeh
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #11  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *      Demo Link: https://drive.google.com/file/d/1KNeCZi4zUDtczsz39PhqKrHS9_mX-uZx/view?usp=sharing
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
unsigned char m = 0x02;

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
			if ((~PINA & 0x0F) == 0x01){
			if ((pattern == p[0]) && (row == 0x7F)){ pattern = p[1]; row =0xBF;} 
			else if ((pattern == p[1]) && (row == 0xBF)){ pattern = p[2]; row = 0xDF;} 
			else { pattern = p[0]; row = 0x7F;} 
			}
			else{
			if ((pattern == p[0]) && (row == r[0])){ pattern = p[1]; row = r[1];} 
			else if ((pattern == p[1]) && (row == r[1])){ pattern = p[2]; row = r[2];} 
			else { pattern = p[0]; row = r[0];} 
			}

/*if (row == 0xF7 && pattern == 0x01) { // Reset demo
pattern = 0x80;
row = 0x7F;
} else if (pattern == 0x01) { // Move LED to start of next row
pattern = 0x80;
row = (row >> 1) | 0x80;
} else { // Shift LED one spot to the right on current row
pattern >>= 1;
}*/
			break;
		default:
			break;
			}
	f = 0x0000;
	f = (f | pattern) << 8; 
	f = (f | row);
	transmit_data(f);
	//transmit_data(0x3CEF);
	//transmit_data(0xFF00);
	return state;
}




enum GoStates {Start_Go, Init_Go, WAIT, INC_P, INC_R, DEC_P,DEC_R};

int GoTick(int state) {

    static unsigned char pattern = 0x3C; // LED pattern - 0: LED off; 1: LED on
    static unsigned char row = 0xBF;     // Row(s) displaying pattern.
					 // 0: display pattern on row
					 // 1: do NOT display pattern on row

    switch(state) { // Transition

	case Start_Go:

		state =Init_Go;

		break;

	case Init_Go:

		state = WAIT;

		break;

	case WAIT:

		if( (~PINA & 0x0F) == 0x01 ) {

			state = INC_P;

		}

		else if ((~PINA & 0x0F) == 0x02){

			state = DEC_P;

		}

		else{

			state = WAIT;

		}

		break;

	case INC_P:

		state = INC_R;

		break;

	case INC_R:

		if( (~PINA & 0x0F) == 0x01 ) {

			state = INC_R;

		}


		else if( (~PINA & 0x0F) == 0x02 ){

			state = DEC_P;

		}

		else{

			state = WAIT;

		}

		break;

	case DEC_P:

		state = DEC_R;

		break;

	case DEC_R:

		if( (~PINA & 0x0F) == 0x02 ) {

			state = DEC_R;

		}

		else if( (~PINA & 0x0F) == 0x01 ){

			state = INC_P;

		}

		else{

			state = WAIT;

		}

		break;



	default:

		state = Start_Go;

		break;

	}


   switch(state) { //Actions

	case Start_Go:
		//row = 0x7F;
		break;

	case Init_Go:
		//row = 0x7F;
		break;

	case WAIT:
		if ((pattern == p[0]) && (row == r[0])){ pattern = p[1]; row = r[1];} 
		else if ((pattern == p[1]) && (row == r[1])){ pattern = p[2]; row = r[2];} 
		else { pattern = p[0]; row = r[0];} 
		break;
	case INC_P:
		
		if( r[0] > 0x7F){
		for (int a = 0; a < 3; a++){
		

			r[a] = (r[a] << 1) | 0x01;
		}
		}
		//else{row = 0x7F;}
		break;

	case INC_R:

		

		break;

	case DEC_P:

		if( r[0] < 0xDF){

		for (int a = 0; a < 3; a++){	
			r[a] = (r[a] >> 1) | 0x80;
			
		}
		}


		break;

	case DEC_R:

		break;

	
	default:
		
		
		row = 0x7F;

		break;

	}
	
	f = 0x0000;
	
	f = (f | pattern) << 8; 
	f = (f | row);
	transmit_data(f);
	//transmit_data(0xFF00);
	return state;
}



enum L_or_R_States {Start_LR, Init_LR, WAIT_LR, R_P, R_R, L_P,L_R};

int L_or_R_Tick(int state) {

    static unsigned char pattern = 0x3C; // LED pattern - 0: LED off; 1: LED on
    static unsigned char row = 0xBF;     // Row(s) displaying pattern.
					 // 0: display pattern on row
					 // 1: do NOT display pattern on row

    switch(state) { // Transition

	case Start_LR:

		state =Init_LR;

		break;

	case Init_LR:

		state = WAIT_LR;

		break;

	case WAIT:

		if( (~PINA & 0x0F) == 0x04 ) {

			state = R_P;

		}

		else if ((~PINA & 0x0F) == 0x08){

			state = L_P;

		}

		else{

			state = WAIT_LR;

		}

		break;

	case R_P:

		state = R_P;

		break;

	case R_R:

		if( (~PINA & 0x0F) == 0x04 ) {

			state = R_R;

		}


		else if( (~PINA & 0x0F) == 0x08 ){

			state = L_P;

		}

		else{

			state = WAIT_LR;

		}

		break;

	case L_P:

		state = DEC_R;

		break;

	case L_R:

		if( (~PINA & 0x0F) == 0x08 ) {

			state = L_R;

		}

		else if( (~PINA & 0x0F) == 0x04 ){

			state = R_P;

		}

		else{

			state = WAIT_LR;

		}

		break;



	default:

		state = Start_Go;

		break;

	}


   switch(state) { //Actions

	case Start_LR:
		//row = 0x7F;
		break;

	case Init_LR:
		//row = 0x7F;
		break;

	case WAIT_LR:
		if ((pattern == p[0]) && (row == r[0])){ pattern = p[1]; row = r[1];} 
		else if ((pattern == p[1]) && (row == r[1])){ pattern = p[2]; row = r[2];} 
		else { pattern = p[0]; row = r[0];} 
		break;
	case R_P:
		
		//if( (p[0] & 0x01) < 0x01){
		if(p[0] == 0xF0){
		for (int a = 0; a < 3; a++){
		

			p[a] = (p[a] >> 1);
		}
		}
		else if(p[0] == 0x78){
		for (int a = 0; a < 3; a++){
		

			p[a] = (p[a] >> 1);
		}
		}
		else if(p[0] == 0x3C){
		for (int a = 0; a < 3; a++){
		

			p[a] = (p[a] >> 1);
		}
		}
		else if(p[0] == 0x1E){
		for (int a = 0; a < 3; a++){
		

			p[a] = (p[a] >> 1);
		}
		}
		break;

	case R_R:

		

		break;

	case L_P:

		//if( (p[0] & 0x08) < 0x80){
		if(p[0] == 0x0F){
		for (int a = 0; a < 3; a++){	
			p[a] = (p[a] << 1);
			
		}
		}
		else if(p[0] == 0x1E){
		for (int a = 0; a < 3; a++){	
			p[a] = (p[a] << 1);
			
		}
		}
		else if(p[0] == 0x3C){
		for (int a = 0; a < 3; a++){	
			p[a] = (p[a] << 1);
			
		}
		}
		else if(p[0] == 0x78){
		for (int a = 0; a < 3; a++){	
			p[a] = (p[a] << 1);
			
		}
		}
		else if(p[0] == 0x1E){
		for (int a = 0; a < 3; a++){	
			p[a] = (p[a] << 1);
			
		}
		}

		break;

	case L_R:

		break;

	
	default:
		
		
		

		break;

	}
	
	f = 0x0000;
	
	f = (f | pattern) << 8; 
	f = (f | row);
	transmit_data(f);
	//transmit_data(0xFF00);
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
     static task task2;
    
    task *tasks[] = { &task1, &task2 };
    
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    unsigned long GCD  = tasks[0]->period;
    const char start = 0;
    task1.state = start;
    task1.period = 1;
    task1.elapsedTime = task1.period;
    //task1.TickFct =&Demo_Tick;
    //task1.TickFct =&GoTick;
      task1.TickFct =&L_or_R_Tick;

    task2.state = start;
    task2.period = 1;
    task2.elapsedTime = task1.period;
   task2.TickFct =&GoTick;
     

    TimerSet(100);//60
    TimerOn();
   

	

    unsigned short i;
    while (1) {
	//0xBF , 0xDF, 0xEF
	//0x3C , 0x24, 0x3C
	//transmit_data(0x017F);
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
