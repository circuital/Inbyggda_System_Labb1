#include <avr/io.h>
#include <util/delay.h>
#include "Serial.h"

void blink_led()
{
	//S�tter bit PBO i port B till output.
	DDRB |= (1 << PB0);
	//S�tter bit PB0 i port B till h�g.
	PORTB |= (1 << PB0);
	_delay_ms(500);
	//S�tter bit PB0 i port B till l�g.
	PORTB &= ~(1 << PB0);
	_delay_ms(500);
}

void uart_init(void)
{
#define F_OSC 16000000 //klockfrekvens
#define BAUD 38400 //baud rate
#define UBRR (F_OSC/(16*BAUD))-1 //baud rate register

	// Shiftar 16bits-v�rdet UBRR 8 steg �t h�ger och s�tter de �vre 8 bitarna till 8bits-registret UBRR0H. 
	UBRR0H = (UBRR >> 8);
	// Kopierar 16bits-v�rdet UBRR till 8bits-registret UBRR0L, de �vre 8 bitarna blir trunkerade, medan de undre 8 bitarna s�tts. 
	UBRR0L = UBRR;
	//Aktiverar USART f�r s�ndning och mottagning.
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	//S�tter frame format: 8data, 1 stop bit.
	UCSR0C = 0b110;
}

void uart_putchar(char c)
{
	//Kollar om tecknet �r lika med \r eftersom Enter i Putty ger \r och s�nder is�fall \n och \r, annars s�nds tecknet. 
	if (c == '\r')
	{
		//V�ntar tills transmitter buffern �r tom.
		while (!(UCSR0A & (1 << UDRE0)));
		//S�nder.
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
	//Loopar �ver varje tecken i str�ngen och kallar p� uart_putchar() med tecknet som argument s� l�nge det �r skilt fr�n \r, d�refter med \r som argument.
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
	//V�ntar tills data har tagits emot. 
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

void uart_echo()
{
	//Kallar p� uart_getchar() och stoppar det returnerade v�rdet i en variabel. Kallar d�refter p� uart_putchar() med variabeln som argument. 
	char c = uart_getchar();
	uart_putchar(c);
}


