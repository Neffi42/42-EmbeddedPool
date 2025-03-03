#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define MAX_VAL 15
#define MIN_VAL 0

uint8_t value = 0;

void printValue() {
	PORTB = 0; // Reset PORTB
    PORTB |= ((value & 0b1) << PORTB0);
	/*
	We need to re-aligned each bit to make it the least-significant bit before setting the appropriate led.
	*/
    PORTB |= (((value & 0b10) >> 1) << PORTB1);
    PORTB |= (((value & 0b100) >> 2) << PORTB2);
    PORTB |= (((value & 0b1000) >> 3) << PORTB4);
}

void isPindXPressed(int pindx) {
	if (!(PIND & (1 << pindx))) {
		if (pindx == PIND2 && value < MAX_VAL) {
			++value;
			printValue();
		}
		else if (pindx == PIND4 && value > MIN_VAL) {
			--value;
			printValue();
		}
		while (!(PIND & (1 << pindx)))
			;
		_delay_ms(50);
	}
}

int main() {
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);
	while (1) {
		isPindXPressed(PIND2);
		isPindXPressed(PIND4);
	}
	return 0;
}
