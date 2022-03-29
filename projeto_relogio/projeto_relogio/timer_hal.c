/*
 * timer_hal.c
 *
 * Created: 21/10/2021 13:13:00
 *  Author: icaro
 */ 

#include "timer_hal.h"

//===================// Timer 0 values //===================//

const uint16_t T0_comp = 250-1;    // compare value for timer 1 
/* 8 bit timer:
 * T1_comp = (time(s) x freq) / prescaler, 
 * for a 1ms interval with a 64 prescaler
 * we have = 0.001 x 16000000 / 64 = 250;
 */ 

//===================// Timer 1 values //===================//

const uint16_t T1_comp = 62500-1;    // compare value for timer 1 
/* 16 bit timer:
 * T1_comp = (time(s) x freq) / prescaler, 
 * for a 12ms interval with a 256 prescaler
 * we have = 0.012 x 16000000 / 256 = 750;
 */ 

void TIM0_init()
{
		//========== TIMER0 =========//
		TCCR0A = 0;
		
		//Prescaler 1:64
		reset_bit(TCCR0B,CS02);	  //0
		set_bit(TCCR0B,CS01);     //1
		set_bit(TCCR0B,CS00);	  //1
		
		//Start and compare value for timer 0
		TCNT0 = 0x00;
		OCR0A = T0_comp;
		
		//Start and compare value for timer 0
		TIMSK0 = (1<<OCIE0A);	
}

void TIM1_init()
{
 	    //========== TIMER1 =========//
 	    TCCR1A = 0;					//unable PWM, compare mode
 	    
 	    //Prescaler 1:256
 	    set_bit(TCCR1B,CS12);       // 1
 	    reset_bit(TCCR1B,CS11);		// 0
 	    reset_bit(TCCR1B,CS10);		// 0
 	    
 	    //Start and compare value for timer1
 	    TCNT1 = 0x00;
 	    OCR1A = T1_comp;
 	    
 	    //Start and compare value for timer1
 	    TIMSK1 = (1 << OCIE1A);	
}