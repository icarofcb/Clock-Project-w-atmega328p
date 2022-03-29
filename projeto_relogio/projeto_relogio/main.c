/*
 *  Title: Infra ATmega328p 
 *			 dd/mm/yyyy
 *  Created: 21/11/2021
 *  Author: ?caro Fernando o brabo dos firmwares
 *	LinkedIn : https://www.linkedin.com/in/?caro-fernando-46139919b/
 *  GitHub   : https://github.com/icarofcb
 */
//===================// Important Libralies //===================//
#include "atmega328p_config.h"
#include "uart_hal.h"
#include "analog_hal.h"
#include "timer_hal.h"

//===================// Digital Ports Hardware Mapping //===================//


//===================// Function prototyping //===================//
void inverte_led();
unsigned long sec_to_time(unsigned long sec);


//===================// Variable Mapping //===================//
unsigned long seconds = 0; // de 0 at? 86400

uint8_t start_flag    = 1;

uint32_t millis    = 0,
		 pastmills = 0;

int h, m, s;	
 
//===================// Timer 0 Interrupt //===================//
ISR(TIMER0_COMPA_vect)
{//1ms each pulse 
	
	if(millis==65535)
	{
		millis = 0;
		pastmills = 0;
	}else if(millis<65535){millis += 1;}
	TCNT0 = 0x00;
	
}

//===================// Timer 1 Interrupt //===================//
ISR(TIMER1_COMPA_vect)
{//1s each pulse
	
	inverte_led();
	
	if(start_flag)
	{
		if(seconds==86400)
		{
			seconds = 0;
		}else if(seconds<86400) seconds++;
	}
			
	TCNT1 = 0x00;                      //resets TIMER1	
}

//===================// Main Code //===================//
int main(void)
{
 	    //========== GPIO =========//
 	    set_bit(DDRB,PORTB5);		 //PB5 as output led onboard;

		
 	    //========== Global interrupt enable =========//
 	    sei();
 
 	    //========== ADC =========//
		analogInit();
		
		//========== UART =========//	
		USART_Init(MYUBRR);
		char str[] = "Clock program\n";
		
		USART_SendString((char *)str);
		
		//========== Timers =========//	
		TIM0_init();
		TIM1_init();
		
 		while(1)
 		{
			if(millis - pastmills>1000)
			{
				if(start_flag)
				{
					sec_to_time(seconds);
				}
				pastmills = millis;
			}
 		}
}

void inverte_led()
{
	invert_bit(PORTB,PORTB5);
}

unsigned long sec_to_time(unsigned long sec)
{
	h = (sec/3600);
	
	m = (sec -(3600*h))/60;
	
	s = (sec -(3600*h)-(m*60));
	
	USART_SendInt(h);
	USART_Transmit(':');
	USART_SendInt(m);
	USART_Transmit(':');
	USART_SendInt(s);
	USART_Transmit('\n');
	
	return 0;
}

