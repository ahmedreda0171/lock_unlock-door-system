 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.c
 *
 * Description: Source file for the External EEPROM Memory
 *
 * 
 *
 *******************************************************************************/
#include "i2c.h"
#include "external_eeprom.h"

void EEPROM_init(void)
{

	TWI_init();
}

uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TW_START)
        return ERROR;
		

    TWI_write((uint8)(0xA0 | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
        return ERROR; 
		 

    TWI_write((uint8)(u16addr));
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;
		

    TWI_write(u8data);
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;


    TWI_stop();
	
    return SUCCESS;
}

uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data)
{

    TWI_start();
    if (TWI_getStatus() != TW_START)
        return ERROR;
		

    TWI_write((uint8)((0xA0) | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
        return ERROR;
		

    TWI_write((uint8)(u16addr));
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;
		

    TWI_start();
    if (TWI_getStatus() != TW_REP_START)
        return ERROR;
		

    TWI_write((uint8)((0xA0) | ((u16addr & 0x0700)>>7) | 1));
    if (TWI_getStatus() != TW_MT_SLA_R_ACK)
        return ERROR;


    *u8data = TWI_readWithNACK();
    if (TWI_getStatus() != TW_MR_DATA_NACK)
        return ERROR;


    TWI_stop();
    return SUCCESS;
}
