#include <avr/io.h>
#include <util/delay.h>

int main() {
	DDRB |= (1 << DDB0);
	while (1) {
		if (!(PIND & (1 << PIND2))) { // Port D is a pull-up register, meaning by default it's set to 1 and to 0 when pressed. | p13 - 1.1.6
			PORTB |= (1 << PORTB0);
			_delay_ms(100); // We add some delay to avoid the spam from the button push.
		} else {
			PORTB &= ~(1 << PORTB0); // We perfom an AND combine to a NOT operation to toggler the bit corresponding to PORTB0.
		}
	}
	return 0;
}
