/**************************************************************
 * Author: Ahmed Reda
 * Date: 1/9/2022
 * keybad.h
 *
 *Description: Header File for the key pad driver
 **************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

/*******************************************************
 *              preprocessor macros
 ******************************************************/

/* keypad configuration for number of rows and columns */
#define N_row 4
#define N_col 4

/* keypad port configuration */
#define KEYPAD_PORT_OUT PORTA
#define KEYPAD_PORT_IN  PINA
#define KEYPAD_PORT_DIR DDRA

/******************************************************
 *             function prototypes                    *
 *****************************************************/

/* function responsible for  getting the pressed keypad key */
uint8 KeyPad_getPressedKey(void);


#endif /* KEYPAD_H_ */
