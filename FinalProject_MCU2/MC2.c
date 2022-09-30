/*********************************************
 * Author: Ahmed Reda                        *
 *                                           *
 * Date: 28/9/2022                           *
 *                                           *
 * file: MC2.c                               *
 *                                           *
 * Description: lock/unlock Door system      *
*********************************************/

#include "micro_config.h"
#include "external_eeprom.h"
#include "uart.h"

uint16 adress[4] = {0x0311,0x0312,0x0313,0x0314};
#define lock_unlock 1
#define init_lock 100
#define lock 10
#define unlock 11
#define change_password 2
#define password_chaneged 21
#define password_not_chaneged 20
#define enter_password 3
#define wrong_password 4
#define correct_password 6
#define lock_system 7
#define unlock_system 8
#define red_led 0
#define green_led 1
#define buzzer 2
#define motor 5

void Timer1_Fast_PWM_Init(unsigned short duty_cycle)
{
	TCNT1 = 0;
	ICR1 = 2499;
	OCR1A = duty_cycle;
	TCCR1A = (1<<WGM11) | (1<<COM1A1);
	TCCR1B = (1<<WGM12) | (1<<WGM13) | (1<<CS10) | (1<<CS11);
}

int main(void)
{
	uint8 password[4] , check_password = 0 , count_error=0;
	uint8 key , entered_password[4] , status=init_lock;

	UART_init(); // initialize UART
	EEPROM_init();

	DDRA |= (1<<green_led)|(1<<red_led)|(1<<buzzer);
	DDRD |= (1<<motor);

	for(int i=0; i<4; i++)
	{
		EEPROM_readByte(adress[i], &password[i]);
	}


	while(1)
	{
		key = UART_recieveByte();

		if(key == lock_unlock)
		{
			if(status != unlock)
			{
				PORTA |= (1<<red_led);
				PORTA &= ~(1<<green_led);
				PORTA &= ~(1<<buzzer);
				check_password=0;

				while(check_password != correct_password)
				{
					status = unlock;

					for(int i=0; i<4; i++)
					{
						entered_password[i] = UART_recieveByte();
					}

					for(int i=0; i<4; i++)
					{
						if(password[i]!=entered_password[i])
						{
							check_password = wrong_password;
							status = wrong_password;
							count_error++;
							break;
						}
					}

					if(count_error==3)
					{
						status=lock_system;
					}

					UART_sendByte(status);

					if(status == unlock)
					{
						count_error=0;
						check_password = correct_password;
						PORTA |= (1<<green_led);
						PORTA &= ~(1<<red_led);
						Timer1_Fast_PWM_Init(250);
					}

					else if(status==lock_system)
					{
						count_error=0;
						PORTA |= (1<<buzzer);

						for(int i=0;i<10;i++)
						{
							PORTA ^= (1<<red_led);
							_delay_ms(1000);
						}

						PORTA &= ~(1<<buzzer);
						status=unlock_system;
						UART_sendByte(status);
					}
				}
			}

			else if(status == unlock)
			{
				status = init_lock;
				PORTA |= (1<<red_led);
				PORTA &= ~(1<<green_led);
				Timer1_Fast_PWM_Init(124);
				UART_sendByte(status);
			}
		}

		else if(key == change_password)
		{
			PORTA |= (1<<red_led);
			PORTA &= ~(1<<green_led);
			PORTA &= ~(1<<buzzer);

			for(int i=0; i<4; i++)
			{
				EEPROM_readByte(adress[i], &password[i]);
				UART_sendByte(password[i]);
			}

			key = UART_recieveByte();

			if(key == password_chaneged)
			{
				for(int i=0; i<4; i++)
				{
					key = UART_recieveByte();
					EEPROM_writeByte(adress[i] , key);
					_delay_ms(10);
				}

				for(int i=0; i<4; i++)
				{
					EEPROM_readByte(adress[i], &password[i]);
				}

				PORTA |= (1<<green_led);
				PORTA &= ~(1<<red_led);
				PORTA &= ~(1<<buzzer);
				_delay_ms(1000);
				PORTA |= (1<<red_led);
			    PORTA &= ~(1<<green_led);
			}

		}
	}


	return 0;
}
