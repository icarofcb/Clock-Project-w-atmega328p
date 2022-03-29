/*
 * atmega328p_config.h
 *
 * Created: 19/10/2021 17:43:33
 *  Author: icaro
 */ 


#ifndef ATMEGA328P_CONFIG_H_
#define ATMEGA328P_CONFIG_H_

#define F_CPU 16000000UL // Clock Speed
#define BAUD 57600
#define MYUBRR F_CPU/16/BAUD-1

//===================// Function defines //===================//

#define set_bit(reg,bit)    (reg |= (1<<bit))   // define bit   "DDRx/PORTx,PORTxn"
#define reset_bit(reg,bit)  (reg &= ~(1<<bit))  // reset bit    "DDRx/PORTx,PORTxn"
#define state_bit(reg,bit)  (reg &  (1<<bit))   // compare bit  "PINx,PORTxn"
#define invert_bit(reg,bit) (reg ^= (1<<bit))   // invert o bit "PORTx,PORTxn"

#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#endif /* ATMEGA328P_CONFIG_H_ */