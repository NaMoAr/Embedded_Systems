/*	Author: Njameh
 *  Partner(s) Name: NONE
 *	Lab Section:022
 *	Assignment: Lab #11  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *      Demo Link: https://drive.google.com/file/d/1KO8NAUdmp4-5J30dH6X_45THfiGq9I2Z/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include <keypad.h>
#include <PWM.h>
#include <scheduler.h>
#endif

void transmit_data(unsigned char data,unsigned char reg){
	int t;
	for (t=0 ; t < 8 ; ++t){
		if (reg== 0x01){PORTC = 0x08;}
		else if (reg== 0x02){PORTC = 0x20;}
		else{PORTC = 0x00;}
		
		PORTC |= ((data >> t ) & 0x01);
		PORTC |= 0x02;
	}
	if (reg == 0x01){PORTC |= 0x04;}
	else if (reg == 0x02){PORTC |= 0x10;}
	else{PORTC |= 0x00;}
	
	PORTC = 0x00;
}


unsigned char go = 0x00;

enum GoStates {Start_Go, Init_Go, WAIT, RESET, INC_P, INC_R, DEC_P,DEC_R};

int GoTick(int state) {

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

		else if ((~PINA & 0x0F) == 0x03){
			
			state = RESET;

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

		else if( (~PINA & 0x0F) == 0x03 ){

			state = RESET;

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

		else if( (~PINA & 0x0F) == 0x03 ){

			state = RESET;

		}

		else if( (~PINA & 0x0F) == 0x01 ){

			state = INC_P;

		}

		else{

			state = WAIT;

		}

		break;

	case RESET:

		if ((~PINA & 0x0F) == 0x03){
			
			state = RESET;

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
		go = 0x01;
		break;

	case Init_Go:
		go = 0x01;
		break;

	case WAIT:
		
		break;
	case INC_P:

		if( go < 0x03){

			go = go + 1;
			
		}

		break;

	case INC_R:

		

		break;

	case DEC_P:

		if( go > 0x01){

			go = go - 1;
			

		}

		break;

	case DEC_R:

		break;

	case RESET:

		if(go == 0x00){go = 0x01;}
		else {go = 0x00;
		      transmit_data(0x00, 0x01);
			}
		

		break;

	default:

		go = 0x00;
		

		break;

	}

	return state;
}




unsigned char j = 0x00;
unsigned char Direction = 0x00;
unsigned char LED = 0x00;

enum festiveOneStates {Start_One, Init_One, One};
int festiveOneTick(int state) {
if (go == 0x01){
	switch(state) {
		case Start_One:
			state = Init_One;
			break;
		case Init_One:
			state = One;
			break;
		case One:
			state = One;
			
                        break;
		}
	switch(state) {
		case Start_One:
			j = 0x00;
			transmit_data(j,0x01);
			break;
		case Init_One:
			j = 0x01;
			transmit_data(j, 0x01);
			break;
		case One:
			if(Direction == 0x00){
					if(j == 0x80){
						
						LED  = (j >> 1);
						j = LED;
						transmit_data(j,0x01);
						Direction = 0x01;
					}
					else{
						LED  = (j << 1);
						j = LED;
						transmit_data(j,0x01);
						
					}
				}
				else{
					if (j == 0x01) {
                                        LED  = j << 1;
					j = LED ;
					transmit_data(j,0x01);
                                        Direction = 0x00;
                               		}
                                	else {
                                        LED = j >> 1;
					j = LED;
					transmit_data(j,0x01);
                                	}

				}
                        break;
		}
		
}
	return state;
}


enum festiveTwoStates {Start_Two, Init_Two, Two};
int festiveTwoTick(int state) {
if(go == 0x02){
	switch(state) {
		case Start_Two:
			state = Init_Two;
			break;
		case Init_Two:
			state = Two;
			break;
		case Two:
			state =Two;
			
                        break;
		}
	switch(state) {
		case Start_Two:
			j = 0x00;
			transmit_data(j,0x01);
			break;
		case Init_Two:
			j = 0x81;
			transmit_data(j,0x01);
			break;
		case Two:
			if(Direction == 0x00){
					if(j == 0x81){
						j = 0xC3;
						transmit_data(j,0x01);
						
					}
					else if (j == 0xC3){
						j = 0xE7;
						transmit_data(j,0x01);
						
					}
					else{
						j = 0xFF;
						transmit_data(j,0x01);
						Direction = 0x01;
					}
			}
			else{
					if(j == 0xC3){
						j = 0x81;
						transmit_data(j,0x01);
						Direction = 0x00;
						
					}
					else if (j ==  0xE7){
						j = 0xC3;
						transmit_data(j,0x01);
						
					}
					else{
						j = 0xE7;
						transmit_data(j,0x01);
						
					}

				}
                        break;
		}
		
}	
	return state;
}



enum festivteThreeStates {Start_Three, Init_Three, Three};

int festiveThreeTick(int state) {
if (go == 0x03){
	switch(state) {
		case Start_Three:
			state = Init_Three;
			break;
		case Init_Three:
			state = Three;
			break;
		case Three:
			state =Three;
			
                        break;
		}
	switch(state) {
		case Start_Three:
			j = 0x00;
			transmit_data(j,0x01);
			break;
		case Init_Three:
			j = 0x81;
			transmit_data(j,0x01);
			break;
		case Three:
			
					if(j == 0x81){
						j = 0x44;
						transmit_data(j,0x01);
						
					}
					else if(j == 0x44){
						j = 0xC3;
						transmit_data(j,0x01);
						
					}
					else if (j == 0xC3){
						j = 0x24;
						transmit_data(j,0x01);
						
					}
					else if (j == 0x24){
						j = 0x66;
						j = 0xE7;
						transmit_data(j,0x01);
						
					}
					else if (j == 0x66){
						j = 0xE7;
						transmit_data(j,0x01);
						
					}
					else if (j == 0xE7){
						j = 0x18;
						transmit_data(j,0x01);
						
					}
					else if (j == 0x18){
						j = 0x99;
						transmit_data(j,0x01);
						
					}
					else if (j == 0x99){
						j = 0xDB;
						transmit_data(j,0x01);
						
					}
					else if (j == 0xDB){
						j = 0xFF;
						transmit_data(j,0x01);
					}
					else {
						j = 0x81;
						transmit_data(j,0x01);
                                        }
			
                        break;
		}
		
}
	return state;
}







unsigned char go_s = 0x00;

enum GoStates_s {Start_Go_s, Init_Go_s, WAIT_s, RESET_s, INC_P_s, INC_R_s, DEC_P_s,DEC_R_s};

int GoTick_s(int state) {

    switch(state) { // Transition

	case Start_Go_s:

		state =Init_Go_s;

		break;

	case Init_Go_s:

		state = WAIT_s;

		break;

	case WAIT_s:

		if( (~PINA & 0x0F) == 0x04 ) {

			state = INC_P_s;

		}

		else if ((~PINA & 0x0F) == 0x08){

			state = DEC_P_s;

		}

		else if ((~PINA & 0x0F) == 0x0C){
			
			state = RESET_s;

		}

		else{

			state = WAIT_s;

		}

		break;

	case INC_P_s:

		state = INC_R_s;

		break;

	case INC_R_s:

		if( (~PINA & 0x0F) == 0x04 ) {

			state = INC_R_s;

		}

		else if( (~PINA & 0x0F) == 0x0C ){

			state = RESET_s;

		}

		else if( (~PINA & 0x0F) == 0x08 ){

			state = DEC_P_s;

		}

		else{

			state = WAIT_s;

		}

		break;

	case DEC_P_s:

		state = DEC_R_s;

		break;

	case DEC_R_s:

		if( (~PINA & 0x0F) == 0x08 ) {

			state = DEC_R_s;

		}

		else if( (~PINA & 0x0F) == 0x0C ){

			state = RESET_s;

		}

		else if( (~PINA & 0x0F) == 0x04 ){

			state = INC_P_s;

		}

		else{

			state = WAIT_s;

		}

		break;

	case RESET_s:

		if ((~PINA & 0x0F) == 0x0C){

			state = RESET_s;

		}

		else{

			state = WAIT_s;

		}

		break;

	default:

		state = Start_Go_s;

		break;

	}


   switch(state) { //Actions

	case Start_Go_s:
		go_s = 0x01;
		break;

	case Init_Go_s:
		go_s = 0x01;
		break;

	case WAIT_s:
		
		break;
	case INC_P_s:

		if( go_s < 0x03){

			go_s = go_s + 1;
			
		}

		break;

	case INC_R_s:

		

		break;

	case DEC_P_s:

		if( go_s > 0x01){

			go_s = go_s - 1;
			

		}

		break;

	case DEC_R_s:

		break;

	case RESET_s:

		if(go_s == 0x00){go_s = 0x01;}
		else {go_s = 0x00;
		      transmit_data(0x00, 0x02);
			}
		

		break;

	default:

		go_s = 0x00;
		

		break;

	}

	return state;
}




unsigned char j_s = 0x00;
unsigned char Direction_s = 0x00;
unsigned char LED_s = 0x00;

enum festiveOneStates_s {Start_One_s, Init_One_s, One_s};
int festiveOneTick_s(int state) {
if (go_s == 0x01){
	switch(state) {
		case Start_One_s:
			state = Init_One_s;
			break;
		case Init_One_s:
			state = One_s;
			break;
		case One_s:
			state = One_s;
			
                        break;
		}
	switch(state) {
		case Start_One_s:
			j_s = 0x00;
			transmit_data(j_s, 0x02);
			break;
		case Init_One_s:
			j_s = 0x01;
			transmit_data(j_s, 0x02);
			break;
		case One_s:
			if(Direction_s == 0x00){
					if(j_s == 0x80){
						
						LED_s  = (j_s >> 1);
						j_s = LED_s;
						transmit_data(j_s, 0x02);
						Direction_s = 0x01;
					}
					else{
						LED_s  = (j_s << 1);
						j_s = LED_s;
						transmit_data(j_s, 0x02);
						
					}
				}
				else{
					if (j_s == 0x01) {
                                        LED_s  = j_s << 1;
					j_s = LED_s ;
					transmit_data(j_s, 0x02);
                                        Direction_s = 0x00;
                               		}
                                	else {
                                        LED_s = j_s >> 1;
					j_s = LED_s;
					transmit_data(j_s, 0x02);
                                	}

				}
                        break;
		}
		
}
	return state;
}


enum festiveTwoStates_s {Start_Two_s, Init_Two_s, Two_s};
int festiveTwoTick_s(int state) {
if(go_s == 0x02){
	switch(state) {
		case Start_Two_s:
			state = Init_Two_s;
			break;
		case Init_Two_s:
			state = Two_s;
			break;
		case Two_s:
			state =Two_s;
			
                        break;
		}
	switch(state) {
		case Start_Two_s:
			j_s = 0x00;
			transmit_data(j_s, 0x02);
			break;
		case Init_Two_s:
			j_s = 0x81;
			transmit_data(j_s, 0x02);
			break;
		case Two_s:
			if(Direction_s == 0x00){
					if(j_s == 0x81){
						j_s = 0xC3;
						transmit_data(j_s, 0x02);
						
					}
					else if (j_s == 0xC3){
						j_s = 0xE7;
						transmit_data(j_s, 0x02);
						
					}
					else{
						j_s = 0xFF;
						transmit_data(j_s, 0x02);
						Direction_s = 0x01;
					}
			}
			else{
					if(j_s == 0xC3){
						j_s = 0x81;
						transmit_data(j_s, 0x02);
						Direction_s = 0x00;
						
					}
					else if (j_s ==  0xE7){
						j_s = 0xC3;
						transmit_data(j_s, 0x02);
						
					}
					else{
						j_s = 0xE7;
						transmit_data(j_s, 0x02);
						
					}

				}
                        break;
		}
		
}	
	return state;
}



enum festiveThreeStates_s {Start_Three_s, Init_Three_s, Three_s};

int festiveThreeTick_s(int state) {
if (go_s == 0x03){
	switch(state) {
		case Start_Three_s:
			state = Init_Three_s;
			break;
		case Init_Three_s:
			state = Three_s;
			break;
		case Three_s:
			state =Three_s;
			
                        break;
		}
	switch(state) {
		case Start_Three_s:
			j_s = 0x00;
			transmit_data(j_s, 0x02);
			break;
		case Init_Three_s:
			j_s = 0x81;
			transmit_data(j_s, 0x02);
			break;
		case Three_s:
			
					if(j_s == 0x81){
						j_s = 0x44;
						transmit_data(j_s, 0x02);
						
					}
					else if(j_s == 0x44){
						j_s = 0xC3;
						transmit_data(j_s, 0x02);
						
					}
					else if (j_s == 0xC3){
						j_s = 0x24;
						transmit_data(j_s, 0x02);
						
					}
					else if (j_s == 0x24){
						j_s = 0x66;
						j_s = 0xE7;
						transmit_data(j_s, 0x02);
						
					}
					else if (j_s == 0x66){
						j_s = 0xE7;
						transmit_data(j_s, 0x02);
						
					}
					else if (j_s == 0xE7){
						j_s = 0x18;
						transmit_data(j_s, 0x02);
						
					}
					else if (j_s == 0x18){
						j_s = 0x99;
						transmit_data(j_s, 0x02);
						
					}
					else if (j_s == 0x99){
						j_s = 0xDB;
						transmit_data(j_s, 0x02);
						
					}
					else if (j_s == 0xDB){
						j_s = 0xFF;
						transmit_data(j_s, 0x02);
					}
					else {
						j_s = 0x81;
						transmit_data(j_s, 0x02);
                                        }
			
                        break;
		}
		
}
	return state;
}









int main() {
    /* Insert DDR and PORT initializations */
    DDRB = 0x7F; PORTB = 0x00;
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    /* Insert your solution below */
    static task task1;
    static task task2;
    static task task3;
    static task task4;
    static task task5;
    static task task6;
    static task task7;
    static task task8;
    task *tasks[] = { &task1 , &task2, &task3, &task4, &task5 , &task6, &task7, &task8};
    
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    unsigned long GCD  = tasks[0]->period;
    const char start = 0;

    task1.state = start;
    task1.period = 500;
    task1.elapsedTime = task1.period;
    task1.TickFct = &GoTick;

    task2.state = start;
    task2.period = 500;
    task2.elapsedTime = task2.period;
    task2.TickFct = GoTick_s;
   

    task3.state = start;
    task3.period = 500;
    task3.elapsedTime = task3.period;
    task3.TickFct = &festiveOneTick;

    task4.state = start;
    task4.period = 500;
    task4.elapsedTime = task4.period;
    task4.TickFct = &festiveOneTick_s;

    task5.state = start;
    task5.period = 500;
    task5.elapsedTime = task5.period;
    task5.TickFct = &festiveTwoTick;
    
    task6.state = start;
    task6.period = 500;
    task6.elapsedTime = task6.period;
    task6.TickFct = &festiveTwoTick_s;

    task7.state = start;
    task7.period = 500;
    task7.elapsedTime = task7.period;
    task7.TickFct = &festiveThreeTick;

    task8.state = start;
    task8.period = 500;
    task8.elapsedTime = task8.period;
    task8.TickFct = &festiveThreeTick_s;
   

    TimerSet(500);
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
