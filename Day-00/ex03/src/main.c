#include <avr/io.h>
#include <util/delay.h>

int main() {
	DDRB |= (1 << DDB0);
	while (1) {
		if (!(PIND & (1 << PIND2))) {
			PORTB ^= (1 << PORTB0); // Here we invert the value of PORTB0 with a XOR op.
			while (!(PIND & (1 << PIND2))) // Infinite loop until the button is released.
				;
			_delay_ms(50); // Here we add some delay to avoid the bouncing effect.
		}
	}
	return 0;
}
