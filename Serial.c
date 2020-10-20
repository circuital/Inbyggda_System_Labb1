#include <avr/io.h>
#include <util/delay.h>
#include "Serial.h"

void blink_led()
{
	//Sätter bit PBO i port B till output
	DDRB |= (1 << PB0);
	//Sätter bit PB0 i port B till hög
	PORTB |= (1 << PB0);
	_delay_ms(500);
	//Sätter bit PB0 i port B till låg
	PORTB &= ~(1 << PB0);
	_delay_ms(500);
}


