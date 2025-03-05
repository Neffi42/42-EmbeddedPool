#include <avr/io.h>
#include <util/delay.h>

#define TOP 62499
#define COMPARE_MATCH 6250

void isPindXPressed(int pindx) {
	if (!(PIND & (1 << pindx))) {
		if (pindx == PIND2 && OCR1A < TOP)
			OCR1A += COMPARE_MATCH;
		else if (pindx == PIND4 && OCR1A > COMPARE_MATCH)
			OCR1A -= COMPARE_MATCH;
		while (!(PIND & (1 << pindx)))
			;
		_delay_ms(50);
	}
}

int main() {
	// Set the Led on output
	DDRB |= (1 << DDB1);

	// Enable fast-PWM / mode 14 | p.142
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);

	// Set TOP value
	ICR1 = TOP;

	// Set the Compare Match to 10% of TOP
	OCR1A = COMPARE_MATCH;

	// Set Compare Output Mode to non-inverting mode
	TCCR1A |= (1 << COM1A1);

	// Start the Timer
	TCCR1B |= (1 << CS12);
	for (;;) {
		isPindXPressed(PIND2);
		isPindXPressed(PIND4);
	}
}
