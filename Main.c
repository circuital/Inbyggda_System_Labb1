#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Serial.h"

void main (void) 
{
	//while (true)
	//{
	//	blink_led();
	//}

	uart_init();

	//while (true)
	//{
	//	uart_putchar('E');
	//	_delay_ms(500);
	//}

	//while (true)
	//{
	//	uart_putstr("Elvira Granqvist\r");
	//	_delay_ms(500);
	//}

	//while (true)
	//{
	//	uart_echo();
	//}

	while (true)
	{
		uart_led();
	}
}
