/*******************************************************
 * Author: Ahmed Reda
 * Date: 1/9/2022
 *
 * lcd.h
 * Module: LCD
 * Description: Header file for the lcd driver
 *
 ******************************************************/

#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/******************************************************
 *                 preprocessor macros                *
 ******************************************************/
/***  LCD Data 4 bits mode configuration          ***/
#define DATA_BITS_MODE 4

/* Use the higher 4 bits in the data port */
#if (DATA_BITS_MODE==4)
#define UPPER_PORT_PINS
#endif

/* lcd HW pins */
#define RS PC0
#define RW PC1
#define E PC2
#define LCD_CTRL_PORT     PORTC
#define LCD_CTRL_PORT_DIR DDRC
#define LCD_DATA_PORT     PORTC
#define LCD_DATA_PORT_DIR DDRC

/* LCD Commands */
#define CLEAR_COMMAND               0x01
#define FOUR_BITS_DATA_MODE         0x02
#define TWO_LINE_LCD_FOUR_BIT_MODE  0x28
#define TWO_LINE_LCD_Eight_BIT_MODE 0x38
#define CURSOR_OFF                  0x0c
#define CURSOR_ON                   0x0f
#define SET_CURSOR_LOCATION         0x80
#define ENTRY_MODE_SET              0x06

/************************************************
 *          functions prototypes                *
 ***********************************************/
void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 data);
void LCD_displayString(const char *str);
void LCD_init(void);
void LCD_clearScreen(void);
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *str);
void LCD_goToRowColumn(uint8 row,uint8 col);
void LCD_intgerToString(sint32 data);
void LCD_SetCGRAMAddress(void);

#endif /* LCD_H_ */
