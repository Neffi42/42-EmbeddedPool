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

void uart_printstr(const char* str) {
	if (!str)
		return;
	for (size_t i = 0; str[i]; i++) {
		uart_tx(str[i]);
	}
}

int main() {
	uart_init();

	// Configure timer 1 for CTC mode.
	TCCR1B |= (1 << WGM12 ) ;

	// Enable CTC interrupt.
	TIMSK1 |= (1 << OCIE1A );

	// Enable global interrupts.
	sei();

	// Set Compare Match to 2Hz at 16MHz with a prescaler of 1024.
	OCR1A = 31249;

	// Start the timer at Fcpu/1024.
	TCCR1B |= (1 << CS10) | (1 << CS12);

	for (;;)
		;
}

ISR(TIMER1_COMPA_vect) {
	uart_printstr("Hello World!\n\r");
}
