#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "emb.h"

#define INPUT_LEN 20
#define BACKSPACE 127

bool READ = false;

typedef enum {
	USER,
	PASS
} input_t;

input_t input = USER;

const char* username = "abasdere";
const char* password = "42";
char input_username[INPUT_LEN + 1] = {0};
char input_password[INPUT_LEN + 1] = {0};

void uart_init() {
	// Set the Baud rate one byte after the other. | p.185
	UBRR0L = (uint8_t)(UBRRADS & 0xFF);
	UBRR0H = (uint8_t)(UBRRADS >> 8);

	// Enable the double speed. | p.182
	UCSR0A |= (1 << U2X0);

	// Enable the Trasmitter and receiver. | p.185
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

	// Enable the USART Receive Complete interrupt.
	UCSR0B |= (1 << RXCIE0);

	// Set frame format: 8data, 1stop bit. | p.
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

char uart_rx() {
	// Wait for data to be received
	while ( !(UCSR0A & (1 << RXC0)) )
		;
	// Get and return received data from buffer
	return UDR0;
}

void uart_tx(char c) {
	// We wait for the transmit buffer to be empty. | p.186
	while (!( UCSR0A & (1 << UDRE0)))
		;
	// We put the data into the buffer, sends the data. | p.186
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

void printValue(uint8_t value) {
	PORTB = 0; // Reset PORTB
    PORTB |= ((value & 0b1) << PORTB0);
    PORTB |= (((value & 0b10) >> 1) << PORTB1);
    PORTB |= (((value & 0b100) >> 2) << PORTB2);
    PORTB |= (((value & 0b1000) >> 3) << PORTB4);
}

void prompt_user(input_t new_input) {
	input = new_input;
	READ = true;
	uart_printstr(input == USER ? "\tusername: " : "\tpassword: ");
	while (READ)
		_delay_ms(20);
	uart_endline(NULL);
}

void write_to_buf(unsigned char c) {
	char* buf = input == USER ? input_username : input_password;
	size_t len = strlen(buf);
	if (len < INPUT_LEN) {
		buf[len] = c;
		buf[len + 1] = '\0';
		uart_tx(input == USER ? c : '*');
	}
}

void handle_backspace() {
	char* buf = input == USER ? input_username : input_password;
	size_t len = strlen(buf);
	if (len > 0) {
		buf[len - 1] = '\0';
		uart_printstr("\b \b");
	}
}

int main() {
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);
	uart_init();
	sei();

	for (;;) {
		memset(input_username, 0, sizeof(input_username));
		memset(input_password, 0, sizeof(input_password));
		uart_endline("Enter your login:");
		prompt_user(USER);
		prompt_user(PASS);
		if (!strcmp(input_username, username) && !strcmp(input_password, password))
			break;
		uart_endline("Bad combination username/password\n");
	}

	uart_printstr("Hello ");
	uart_printstr(username);
	uart_endline("!\n\rShall we play a game?");

	for (uint8_t i = 0;; i++) {
		printValue(i);
		_delay_ms(300);
	}
}

ISR(USART_RX_vect) {
	if (!READ)
		return;

	unsigned char c = UDR0;
	if (c == '\r') {
		READ = false;
	} else if (c == BACKSPACE) {
		handle_backspace();
	} else {
		write_to_buf(c);
	}
}
