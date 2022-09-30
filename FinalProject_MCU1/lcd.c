/******************************************************
 *  Author: Ahmed Reda
 *  Date: 1/9/2022
 * lcd.c                                              *                                                 *
 *  Module: LCD.c
 *  Date: 1/9/2022                                    *
 *  Description: source file for the LCD driver       *
 *****************************************************/

#include "lcd.h"

/***********************************************
 *        Functions Definations                *
 **********************************************/

void LCD_init(void)
{
	LCD_CTRL_PORT_DIR |= (1<<E) | (1<<RS) | (1<<RW);
    #if(DATA_BITS_MODE == 4)
       #ifdef UPPER_PORT_PINS
	      LCD_DATA_PORT_DIR |= 0xF0;
       #else
	      LCD_DATA_PORT_DIR |= 0x0F;
       #endif
	   LCD_sendCommand(FOUR_BITS_DATA_MODE);
	   _delay_ms(35);
	   LCD_sendCommand(TWO_LINE_LCD_FOUR_BIT_MODE);
    #elif (DATA_BITS_MODE == 8)
	      LCD_DATA_PORT_DIR |= 0xFF;
	      _delay_ms(35);
	      LCD_sendCommand(TWO_LINE_LCD_Eight_BIT_MODE);
    #endif
	_delay_us(40);
	LCD_sendCommand(CURSOR_ON);
	_delay_us(40);
	LCD_sendCommand(CLEAR_COMMAND);
	_delay_ms(2);
	LCD_sendCommand(ENTRY_MODE_SET);
}

/*****   LCD_sendCommand    *******************/

void LCD_sendCommand(uint8 command)
{
	CLEAR_BIT(LCD_CTRL_PORT,RS);
	CLEAR_BIT(LCD_CTRL_PORT,RW);
#if (DATA_BITS_MODE == 4)

	/* out the highest 4 bits of the required command to the data bus D4 --> D7 */
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0f)\
		   | (command & 0xf0);
#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0)\
	  	   		   | ((command & 0xf0)>>4);
#endif

	SET_BIT(LCD_CTRL_PORT,E);
    _delay_ms(1);
    CLEAR_BIT(LCD_CTRL_PORT,E);

    /* out the lowest 4 bits of the required command to the data bus D4 --> D7 */
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0f)\
		   | ((command & 0x0F) << 4);
#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0)\
	  	   		   | (command & 0x0F);
#endif

	SET_BIT(LCD_CTRL_PORT,E);
    _delay_ms(1);
    CLEAR_BIT(LCD_CTRL_PORT,E);

#elif (DATA_BITS_MODE == 8)
    LCD_DATA_PORT = command;
    SET_BIT(LCD_CTRL_PORT,E);
    _delay_ms(1);
    CLEAR_BIT(LCD_CTRL_PORT,E);
#endif
}

/****  LCD_displayCharacter    *******************/


void  LCD_displayCharacter(uint8 data)
{
	SET_BIT(LCD_CTRL_PORT,RS);
	CLEAR_BIT(LCD_CTRL_PORT,RW);
#if (DATA_BITS_MODE == 4)

	/* out the highest 4 bits of the required data to the data bus D4 --> D7 */
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0f)\
		   | (data & 0xf0);
#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0)\
	  	   		   | ((data & 0xf0)>>4);
#endif

	SET_BIT(LCD_CTRL_PORT,E);
    _delay_ms(1);
    CLEAR_BIT(LCD_CTRL_PORT,E);

    /* out the lowest 4 bits of the required data to the data bus D4 --> D7 */
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0f)\
		   | ((data & 0x0F)<<4);
#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0)\
	  	   		   | (data & 0x0f);
#endif

	SET_BIT(LCD_CTRL_PORT,E);
    _delay_ms(1);
    CLEAR_BIT(LCD_CTRL_PORT,E);

#elif (DATA_BITS_MODE == 8)
    LCD_DATA_PORT = command;
    SET_BIT(LCD_CTRL_PORT,E);
    _delay_ms(1);
    CLEAR_BIT(LCD_CTRL_PORT,E);
#endif
}


/******   LCD_displayString     ******************/

void LCD_displayString(const char *str)
{
	while(*str != '\0')
	{
		LCD_displayCharacter(*str);
		str++;
	}
}

/****      LCD_goToRowColumn   *****************/

void LCD_goToRowColumn(uint8 row , uint8 col)
{
	uint8 address;
	if(col<=39)
	{
		switch(row)
		{
		case 0:
			address = SET_CURSOR_LOCATION | col;
			break;
		case 1:
			address = SET_CURSOR_LOCATION | (col+0x40);
			break;
		case 2:
			address = SET_CURSOR_LOCATION | (col+0x10);
			break;
		case 3:
			address = SET_CURSOR_LOCATION | (col+0x50);
			break;
		default:
			break;
		}
		LCD_sendCommand(address);
	}
}

/*********   LCD_displayStringRowColumn  *******/
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str)
{
	LCD_goToRowColumn(row,col); /* go to to the required LCD position */
	LCD_displayString(Str); /* display the string */
}

/********     LCD_intgerToString    *****************/
void LCD_intgerToString(sint32 num)
{
	uint8 arr[10] ,i=0, j ;
		sint16 sign = num;
		sint16 s , k;

		if(num<0)          //check sign
		{
			num=-num;
		}

		do{     // convert num to ASCI in reverse order
			arr[i]= num%10 + '0';
			i++;
		}while((num = (num-num%10)/10)>0);

		if(sign<0)        // put sign
			{
			arr[i++]= '-';
			}
		j=i-1;   k=i;

		for(i=0; i<j; i++,j--)        // Reverse array to put number in order
		{
			s = arr[i];
			arr[i] = arr[j];
			arr[j] = s;
		}

			for( j=0; j<k; j++ )        // printing the number
		{
				LCD_displayCharacter(arr[j]);
		}
}

/****** LCD_clearScreen  ***************************/

void LCD_clearScreen(void)
{
	LCD_sendCommand(CLEAR_COMMAND); //clear display
}

///////////////  (CG RAM)caracter generator  ///////////////////////////////////////////

void LCD_SetCGRAMAddress(void)
{
	LCD_sendCommand(0b01000000);

	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00001000);
	LCD_displayCharacter(0b00001000);
	LCD_displayCharacter(0b00001000);
	LCD_displayCharacter(0b00001000);
	LCD_displayCharacter(0b00001001);
	LCD_displayCharacter(0b00001111);
	LCD_displayCharacter(0b00000000);

	LCD_sendCommand(0b01001000);

	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000100);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000110);
	LCD_displayCharacter(0b00001001);
	LCD_displayCharacter(0b00011111);
	LCD_displayCharacter(0b00000000);

	LCD_sendCommand(0b01010000);

	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000001);
	LCD_displayCharacter(0b00000010);
	LCD_displayCharacter(0b00011100);

	LCD_sendCommand(0b01011000);

	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);

	LCD_sendCommand(0b01100000);

	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000001);
	LCD_displayCharacter(0b00000001);
	LCD_displayCharacter(0b00000001);
	LCD_displayCharacter(0b00011111);
	LCD_displayCharacter(0b00000000);
	LCD_sendCommand(0b01101000);

	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000111);
	LCD_displayCharacter(0b00111101);
	LCD_displayCharacter(0b00000111);

	LCD_sendCommand(0b01110000);

	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00001111);
	LCD_displayCharacter(0b00000001);
	LCD_displayCharacter(0b00011111);
	LCD_displayCharacter(0b00000000);

	LCD_sendCommand(0b01111000);

	LCD_displayCharacter(0b00000111);
	LCD_displayCharacter(0b00000100);
	LCD_displayCharacter(0b00001111);
	LCD_displayCharacter(0b00000000);
	LCD_displayCharacter(0b00000100);
	LCD_displayCharacter(0b00000100);
	LCD_displayCharacter(0b00000100);
	LCD_displayCharacter(0b00000100);
}
