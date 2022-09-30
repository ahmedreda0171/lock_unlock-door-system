 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 *
 *
 *******************************************************************************/

#include "uart.h"

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 8UL))) - 1)

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void UART_init(void)
{

	UCSRA = (1<<U2X);

	UCSRB = (1<<RXEN) | (1<<TXEN);
	

	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1); 
	

	UBRRH = BAUD_PRESCALE>>8;
	UBRRL = BAUD_PRESCALE;
}
	
void UART_sendByte(const uint8 data)
{

	while(BIT_IS_CLEAR(UCSRA,UDRE)){}

	UDR = data;

}

uint8 UART_recieveByte(void)
{

	while(BIT_IS_CLEAR(UCSRA,RXC)){}

    return UDR;		
}

void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}

}

void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;
	Str[i] = UART_recieveByte();
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}
	Str[i] = '\0';
}
