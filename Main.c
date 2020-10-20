#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Serial.h"

void main (void) 
{
	while (true)
	{
		blink_led();
	}
}
