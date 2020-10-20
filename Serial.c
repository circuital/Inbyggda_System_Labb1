#include <avr/io.h>
#include <util/delay.h>
#include "Serial.h"

void blink_led()
{
	//S�tter bit PBO i port B till output
	DDRB |= (1 << PB0);
	//S�tter bit PB0 i port B till h�g
	PORTB |= (1 << PB0);
	_delay_ms(500);
	//S�tter bit PB0 i port B till l�g
	PORTB &= ~(1 << PB0);
	_delay_ms(500);
}


