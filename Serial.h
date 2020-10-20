#ifndef SERIAL_H_
#define SERIAL_H_

void blink_led(void);

void uart_init(void);

void uart_putchar(char c);

void uart_putstr(const char* s);

char uart_getchar(void);

void uart_echo(void);

void uart_led(void);

#endif
