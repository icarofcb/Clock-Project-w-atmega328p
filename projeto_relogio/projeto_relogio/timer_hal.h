/*
 * timer_hal.h
 *
 * Created: 21/10/2021 13:12:48
 *  Author: icaro
 */ 


#ifndef TIMER_HAL_H_
#define TIMER_HAL_H_

#include "atmega328p_config.h"

#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void TIM0_init();
void TIM1_init();

#endif /* TIMER_HAL_H_ */