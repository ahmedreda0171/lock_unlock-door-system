/*******************************************************
 * Author: Ahmed Reda
 *
 * Date: 24/8/2022
 *
 * common_macros.h
 *
 *  Description: commonly used Macros
 *******************************************************/

#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

/* set a certain bit in any register */
#define SET_BIT(REG,BIT) (REG|=(1<<BIT))

/* clear a certain bit in any register */
#define CLEAR_BIT(REG,BIT) (REG&=(~(1<<BIT)))

/* toggle a certain bit in any register */
#define TOGGLE_BIT(REG,BIT) (REG^=(1<<BIT))

/* rotate right the register value with a specific number of rotate */
#define ROR(REG,num) ( REG = (REG>>num) | (REG<<(8-num)) )

/* rotate left the register value with a specific number of rotate */
#define ROL(REG,num) ( REG = (REG<<num) | (REG>>(8-num)) )

/* check if a specific bit in any register is set and return true if yes */
#define BIT_IS_SET(REG,num) (REG & (1<<num))

/* check if a specific bit in any register is clear and return true if yes */
#define BIT_IS_CLEAR(REG,num) ( !(REG & (1<<num)) )


#endif /* COMMON_MACROS_H_ */
