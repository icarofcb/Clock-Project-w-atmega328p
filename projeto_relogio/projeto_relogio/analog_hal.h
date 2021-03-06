/*
 *  Title: ADC SERIAL TIMER for general AVR
 *
 *			 dd/mm/yyyy
 *  Created: 10/18/2021 9:22:18 AM
 *  Author: ?caro Fernando
 *	LinkedIn : https://www.linkedin.com/in/?caro-fernando-46139919b/
 *  GitHub   : https://github.com/icarofcb
 *
 */ 

#ifndef ANALOG_HAL_H_
#define ANALOG_HAL_H_



//===================// Important Libralies //===================//

#include "atmega328p_config.h"

#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>

//===================// Function prototyping //===================//

int16_t analogRead();                     //ADC 
void analogInit();

#endif /* ANALOG_HAL_H_ */