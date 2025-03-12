#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "emb.h"

#define INPUT_LEN 7
#define BACKSPACE 127
#define HEX_BASE "0123456789ABCDEFabcdef"
#define CLEAR_STR "\x1b[2J\x1b[H"

bool READ = false;

char buf[INPUT_LEN + 1] = {0};

void uart_init() {
	UBRR0L = (uint8_t)(UBRRADS & 0xFF);
	UBRR0H = (uint8_t)(UBRRADS >> 8);

	UCSR0A |= (1 << U2X0);

	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

	UCSR0B |= (1 << RXCIE0);

	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

char uart_rx() {
	while ( !(UCSR0A & (1 << RXC0)) )
		;
	return UDR0;
}

void uart_tx(char c) {
	while (!( UCSR0A & (1 << UDRE0)))
		;
	UDR0 = c;
}

void uart_printstr(const char* str) {
	if (!str)
		return;
	for (size_t i = 0; str[i]; i++) {
		uart_tx(str[i]);
	}
}

void uart_endline(const char* str) {
	if (str)
		uart_printstr(str);
	uart_printstr("\n\r");
}

void uart_clear() {
	uart_printstr(CLEAR_STR);
}

void write_to_buf(unsigned char c) {
	size_t len = strlen(buf);
	if (len < INPUT_LEN) {
		buf[len] = c;
		buf[len + 1] = '\0';
		uart_tx(c);
	}
}

void handle_backspace() {
	size_t len = strlen(buf);
	if (len > 0) {
		buf[len - 1] = '\0';
		uart_printstr("\b \b");
	}
}

void init_rgb() {
	DDRD |= (1 << DDD3) | (1 << DDD6) | (1 << DDD5);

	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	TCCR0B |= (1 << CS00);


	TCCR2A |= (1 << COM2B1);
	TCCR2A |= (1 << WGM21) | (1 << WGM20);
	TCCR2B |= (1 << CS20);

	OCR0B = 0;
	OCR0A = 0;
	OCR2B = 0;
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

void prompt_user() {
	uart_endline("Enter an HEX RGB color with the format #RRGGBB:");

	READ = true;
	while (READ)
		_delay_ms(20);

	uart_endline(NULL);
}

void parse_rgb() {
	if (buf[0] != '#' || strlen(buf) != INPUT_LEN || strspn(buf + 1, HEX_BASE) != INPUT_LEN - 1) {
		uart_endline("Error: bad format.");
		return set_rgb(0, 0, 0);
	}
	uint8_t r = strtol(buf + 1, NULL, 16) >> 16;
	uint8_t g = strtol(buf + 3, NULL, 16) >> 8;
	uint8_t b = strtol(buf + 5, NULL, 16);
	set_rgb(r, g, b);
}

int main() {
	uart_init();
	init_rgb();
	sei();

	for (;;) {
		memset(buf, 0, sizeof(buf));
		prompt_user();
		parse_rgb();
	}
}

ISR(USART_RX_vect) {
	if (!READ)
		return;

	unsigned char c = UDR0;
	if (c == '\r') {
		uart_clear();
		READ = false;
	} else if (c == BACKSPACE) {
		handle_backspace();
	} else {
		write_to_buf(c);
	}
}
