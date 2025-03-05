#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stddef.h>
#include "emb.h"

void uart_init() {
	// Set the Baud rate one byte after the other. | p.185
	UBRR0L = (uint8_t)(UBRRADS & 0xFF);
	UBRR0H = (uint8_t)(UBRRADS >> 8);

	// Enable the double speed. | p.182
	UCSR0A |= (1 << U2X0);

	// Enable the Trasmitter and receiver. | p.185
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

	// Set frame format: 8data, 1stop bit. | p.
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

void uart_tx(char c) {
	// We wait for the transmit buffer to be empty. | p.186
	while (!( UCSR0A & (1 << UDRE0)))
		;
	// We put the data into the buffer, sends the data. | p.186
	UDR0 = c;
}

char uart_rx() {
	// Wait for data to be received
	while ( !(UCSR0A & (1 << RXC0)) )
		;
	// Get and return received data from buffer
	return UDR0;
}

int main() {
	uart_init();
	uart_tx(uart_rx());
}
