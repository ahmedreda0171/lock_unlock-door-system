/*********************************************
 * Author: Ahmed Reda                        *
 *                                           *
 * Date: 28/9/2022                           *
 *                                           *
 * file: MC1.c                               *
 *                                           *
 * Description: lock/unlock Door system      *
*********************************************/

#include"lcd.h"
#include"keypad.h"
#include "uart.h"

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


int main()
{
	UART_init(); // initialize UART
	LCD_init();
	uint8 key , entered_password[4] , old_password[4];
	uint8 status = init_lock , count_error=0 , password_check=0;

	while(1)
	{
		LCD_clearScreen();
		LCD_intgerToString(1);
		LCD_displayString("-(un)/lock door.");

		LCD_goToRowColumn(1,0);
		LCD_intgerToString(2);
		LCD_displayString("-change password.");

		key = KeyPad_getPressedKey();

		if(key == 1)
		{
			UART_sendByte(lock_unlock);

			if(status != unlock)
			{
				while(status != unlock)
				{
					if(status == init_lock)
					{
						LCD_clearScreen();
				        LCD_displayString("Enter password:");
					    LCD_goToRowColumn(1,0);
					}

					else if(status == wrong_password)
					{
						LCD_clearScreen();
						LCD_displayString("Wrong!(Try again)");
						LCD_goToRowColumn(1,0);
					}

					else if(status == lock_system)
					{
						LCD_clearScreen();
						LCD_displayString("lock_system");
						LCD_goToRowColumn(1,0);
						LCD_displayString("for");
						LCD_goToRowColumn(1,4);
						LCD_intgerToString(10);
						LCD_displayString(" sec");

						status = UART_recieveByte();

						LCD_clearScreen();
				        LCD_displayString("Enter password:");
					    LCD_goToRowColumn(1,0);
					}

				    for(int i=0;i<4;i++)
				    {
				    	_delay_ms(300);
						key = KeyPad_getPressedKey();
						LCD_intgerToString(key);
					    entered_password[i] = key;
				    }

					for(int i=0; i<4; i++)
					{
						UART_sendByte(entered_password[i]);
					}

					status = UART_recieveByte();

					 if(status == unlock)
					{
						LCD_clearScreen();
						LCD_displayString("success");
						LCD_goToRowColumn(1,0);
						_delay_ms(1000);
					}
				}
			}

			else if(status == unlock)
			{
				LCD_clearScreen();
				LCD_displayString("locked");
				_delay_ms(1000);
				status = UART_recieveByte();
			}

		}


		else if(key == change_password)
		{
			UART_sendByte(change_password);

			for(int i=0; i<4; i++)
			{
				old_password[i] = UART_recieveByte();
			}

		    while((count_error<3) && (password_check!=1))
		    {
		    	password_check = 1;
				LCD_clearScreen();
				LCD_displayString("Enter old");
			    LCD_goToRowColumn(1,0);
			    LCD_displayString("password: ");

			    for(int i=0;i<4;i++)
			    {
			    	_delay_ms(300);
					key = KeyPad_getPressedKey();
					LCD_intgerToString(key);
				    entered_password[i] = key;
			    }
				for(int i=0; i<4; i++)
				{
					if(old_password[i]!=entered_password[i])
					{
						count_error++;

						if(count_error==3)
						{
							password_check = 0;
							LCD_clearScreen();
							LCD_displayString("lock_system");
							LCD_goToRowColumn(1,0);
							LCD_displayString("for");
						    LCD_goToRowColumn(1,4);
							LCD_intgerToString(10);
							LCD_displayString(" sec");
							_delay_ms(10000);
							break;
						}
						else
						{
							password_check = 0;
							LCD_clearScreen();
							LCD_displayString("wrong!");
							_delay_ms(1000);
							break;
						}
					}
				}
		    }

			if(password_check==1)
			{
				password_check=0;
				count_error = 0;
				UART_sendByte(password_chaneged);
				LCD_clearScreen();
				LCD_displayString("new password:");
			    LCD_goToRowColumn(1,0);

			    for(int i=0;i<4;i++)
			    {
			    	_delay_ms(300);
					key = KeyPad_getPressedKey();
					LCD_intgerToString(key);
					UART_sendByte(key);
			    }
			    LCD_displayString("  done");
			    _delay_ms(1000);
			}
			else
			{
				count_error = 0;
				UART_sendByte(password_not_chaneged);
			}


		}
	}


	return 0;
}
