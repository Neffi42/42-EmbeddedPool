#include <avr/io.h>
#include <stdint.h>

typedef volatile uint16_t vu16_t;

void delay(uint16_t ms) {
	for (vu16_t i = 0; i < ms; i++)
		for (vu16_t j = 0; j < 1000; j++)
			;
}

int main() {
	DDRB |= (1 << DDB1);
	while (1) {
		PORTB ^= (1 << PORTB1);
		delay(500);
	}
}
