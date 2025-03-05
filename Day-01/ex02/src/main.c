#include <avr/io.h>

int main() {
	// Set the Led on output
	DDRB |= (1 << DDB1);

	// Enable fast-PWM / mode 14 | p.142
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);

	// Set TOP value
	ICR1 = 62499;

	// Set the Compare Match to 10% of TOP
	OCR1A = 6250;

	// Set Compare Output Mode to non-inverting mode
	TCCR1A |= (1 << COM1A1);

	// Start the Timer
	TCCR1B |= (1 << CS12);
	for (;;)
		;
}
