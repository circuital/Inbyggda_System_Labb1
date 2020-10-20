#include <avr/io.h>
#include <util/delay.h>
#include "Serial.h"

void blink_led()
{
	//Sätter bit PBO i port B till output.
	DDRB |= (1 << PB0);
	//Sätter bit PB0 i port B till hög.
	PORTB |= (1 << PB0);
	_delay_ms(500);
	//Sätter bit PB0 i port B till låg.
	PORTB &= ~(1 << PB0);
	_delay_ms(500);
}

void uart_init(void)
{
#define F_OSC 16000000 //klockfrekvens
#define BAUD 38400 //baud rate
#define UBRR (F_OSC/(16*BAUD))-1 //baud rate register

	// Shiftar 16bits-värdet UBRR 8 steg åt höger och sätter de övre 8 bitarna till 8bits-registret UBRR0H. 
	UBRR0H = (UBRR >> 8);
	// Kopierar 16bits-värdet UBRR till 8bits-registret UBRR0L, de övre 8 bitarna blir trunkerade, medan de undre 8 bitarna sätts. 
	UBRR0L = UBRR;
	//Aktiverar USART för sändning och mottagning.
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	//Sätter frame format: 8data, 1 stop bit.
	UCSR0C = 0b110;
}

void uart_putchar(char c)
{
	//Kollar om tecknet är lika med \r eftersom Enter i Putty ger \r och sänder isåfall \n och \r, annars sänds tecknet. 
	if (c == '\r')
	{
		//Väntar tills transmitter buffern är tom.
		while (!(UCSR0A & (1 << UDRE0)));
		//Sänder.
		UDR0 = '\n';

		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = '\r';
	}
	else
	{
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = c;
	}
}

void uart_putstr(const char* s)
{
	//Loopar över varje tecken i strängen och kallar på uart_putchar() med tecknet som argument så länge det är skilt från \r, därefter med \r som argument.
	int i = 0;
	while (s[i] != '\r')
	{
		uart_putchar(s[i]);
		i++;
	}
	uart_putchar('\r');
}

char uart_getchar()
{
	//Väntar tills data har tagits emot. 
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

void uart_echo()
{
	//Kallar på uart_getchar() och stoppar det returnerade värdet i en variabel. Kallar därefter på uart_putchar() med variabeln som argument. 
	char c = uart_getchar();
	uart_putchar(c);
}

void uart_led()
{
	DDRB |= (1 << PB0);

	char c;
	char s[10];
	int i = 0;

	//Loopar över de inkommande tecknen och kallar på uart_putchar() med tecknet som argument så länge det är skilt från \r samt lägger tecknet i en sträng. 
	while ((c = uart_getchar()) != '\r')
	{
		uart_putchar(c);
		s[i] = c;
		i++;
	}
	uart_putchar('\r');

	//Lägger till \r och \n i strängen samt noll-terminerar den för att kunna använda strcmp().
	s[i] = '\r';
	s[i + 1] = '\n';
	s[i + 2] = '\0';

	//Sätter bit PB0 i port B till hög om strängen är lika med "ON\r\n".
	if (strcmp(s, "ON\r\n") == 0)
	{
		PORTB |= (1 << PB0);
	}
	//Sätter bit PB0 i port B till låg om strängen är lika med "OFF\r\n".
	else if (strcmp(s, "OFF\r\n") == 0)
	{
		PORTB &= ~(1 << PB0);
	}
}


