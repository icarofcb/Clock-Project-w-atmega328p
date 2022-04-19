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
void bt1();
void bt2();
void configTime();
void printTime();
unsigned long sec_to_time(unsigned long sec);
int adcToTime(char digitToConfig);

//===================// Variable Mapping //===================//
unsigned long unixTime = 0; // de 0 ate 86400
unsigned long h=0, m=0,s=0;	//hora, minuto, segundos

volatile uint8_t start_flag = 0;

volatile char timeDigitConfig = 's'; //s = Seconds, m = Minutes, h = Hour;

uint16_t millis    = 0,
		 pastmills = 0;


 
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
		if(unixTime==86400)
		{
			unixTime = 0;
		}else if(unixTime<86400) unixTime++;
		printTime();
	}
			
	TCNT1 = 0x00;                      //resets TIMER1	
}

//===================// INT0 Interrupt //===================//
ISR(INT0_vect)
{
	bt1(); //modo do relogio, rodando ou configurando
}

//===================// INT1 Interrupt //===================//
ISR(INT1_vect)
{
	bt2(); //digito a ser configurado
}

//===================// Main Code //===================//
int main(void)
{
 	    //========== GPIO =========//
 	    set_bit(DDRD,PORTD4);		 //PB5 as output led onboard;
		
		reset_bit(DDRD,PORTD2);      //D2 as input
		reset_bit(DDRD,PORTD3);      //D3 as input
		
		set_bit(PORTD,PORTD2);       //D2 as pull-up
		set_bit(PORTD,PORTD3);		 //D3 as pull-up
		
		//========== INT =========//
		set_bit(EICRA,ISC01);        //the falling edge of INT0 generates an interrupt request
		set_bit(EICRA,ISC11);        //the falling edge of INT1 generates an interrupt request
		
		set_bit(EIMSK,INT0);         //enables INT0
		set_bit(EIMSK,INT1);         //enables INT1
		
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
			if(start_flag)
			{
				if(millis - pastmills>1000)
				{
					sec_to_time(unixTime);
					pastmills = millis;
				}
			}else
			{
				 configTime();
			}
			
			_delay_ms(1);
 		}
}

void inverte_led()
{
	invert_bit(PORTD,PORTD4);
}

unsigned long sec_to_time(unsigned long sec)
{	
	h = (sec/3600);
	
	m = (sec -(3600*h))/60;
	
	s = (sec -(3600*h)-(m*60));
	
	return 0;
}

void printTime()
{
	USART_SendInt(h);
	USART_Transmit(':');
	
	USART_SendInt(m);
	USART_Transmit(':');
	
	USART_SendInt(s);
	USART_Transmit(':');
	
	USART_Transmit('\n');
}
void bt1()
{
	static char clockMode[] =  "\nContando Tempo\n";
	static char configMode[] = "\nConfigure o Tempo\n";
	
	start_flag = start_flag? 0 : 1;
	
	if(start_flag)
	{
		unixTime = s + m*60 + h*3600;
		USART_SendString((char *)clockMode);
	}else{
		USART_SendString((char *)configMode);
	}
	
}

void bt2()
{
		switch(timeDigitConfig)
		{
			case 's':
				timeDigitConfig = 'm';
			break;
			
			case 'm':
				timeDigitConfig = 'h';
			break;
			
			case 'h':
				timeDigitConfig = 's';
			break;
			
			default:
			break;
		}

	
}

void configTime()
{
	uint8_t seconds = 0, minutes = 0, hours = 0;
	char str0 [] = "Digito: ";
	char str1 [] = "Botao: escolhe digito || Touch: Play";
	 
	switch(timeDigitConfig)
	{
		case 's':
			seconds = adcToTime(timeDigitConfig);
			s = seconds;
			break;
			
		case 'm':
			minutes = adcToTime(timeDigitConfig);
			m = minutes;
			break;
			
		case 'h':
			hours = adcToTime(timeDigitConfig);
			h = hours;
			break;
			
		default: 
			break;
	}
	
	USART_SendInt(analogRead(0));
	USART_Transmit('\t');
	USART_SendString((char *)str0);
	USART_Transmit(':');
	USART_Transmit(timeDigitConfig);
	USART_Transmit('\t');
	USART_SendInt(h);
	USART_Transmit(':');
	USART_SendInt(m);
	USART_Transmit(':');
	USART_SendInt(s);
	USART_Transmit('\t');
	USART_SendString((char *) str1);
	USART_Transmit('\n');
	
	_delay_ms(50);
}


int adcToTime(char digitToConfig)
{
	float seconds = 0, minutes = 0, hours = 0;
	uint8_t result = 0;
	
		switch(digitToConfig)
		{
			case 's':
				seconds = (float)analogRead(0) * 59/1023;
				result = seconds;
			break;
			
			case 'm':
				minutes = (float)analogRead(0) * 59/1023;
				result = minutes;
			break;
			
			case 'h':
				hours = (float)analogRead(0) * 23/1023;
				result = hours;
			break;
			
			default:
			break;
		}
		
		return result;
}
