#include <avr/io.h>

int main() {
	DDRB |= (1 << DDB1); // Set pin B1 to output.
	TCCR1B |= (1 << WGM12); // Enable the Clear Timer on Compare (CTC) mode. It will compare the timer with OCR1A. | p.131
	/*
	We set the timer's target. | p.140
	OCR1A = (CPU_Clock / (Prescaler * Desired_Frequency)) - 1
	31249 = (16000000 / (256 * 2)) - 1
	Whe use a 2Hz frequency because we have two operations and not one.
	*/
	OCR1A = 31249;
	TCCR1A |= (1 << COM1A0); // Set Compare match output to toggle OC1A bit (pin B1). | p.140
	TCCR1B |= (1 << CS12); // Set the prescaler. | p.143
	for (;;)
		;
}
