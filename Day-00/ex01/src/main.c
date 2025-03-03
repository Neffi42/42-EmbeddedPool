#include <avr/io.h>

int main() {
	/* p85 - 14.2.1
	DDRB is in charge of the the direction of the pin B0 (input / ouput). Here we set it to output.
	*/
	DDRB |= (1 << DDB0);
	/* p85 - 14.2.1
	If the pin is configured as an ouput pin and we set PORTB0 to 1, then the port pin is driven high (which will turn on the led).
	*/
	PORTB |= (1 << PORTB0);
	return 0;
}
