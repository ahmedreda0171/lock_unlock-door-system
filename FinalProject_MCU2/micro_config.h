/*************************************************************
 * Author: Ahmed Reda
 * Date: 24/8/2022
 * micro_config.h
 *
 *Description: file include all micro controller libraries
**************************************************************/

#ifndef MICRO_CONFIG_H_
#define MICRO_CONFIG_H_

#ifndef F_CPU
#define F_CPU 1000000UL  /* 1 MHZ clock frequency */
#endif

#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>


#endif /* MICRO_CONFIG_H_ */
