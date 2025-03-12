#include <avr/io.h>
#include <util/delay.h>

int main() {
	DDRD |= (1 << DDD3) | (1 << DDD6) | (1 << DDD5);
	for (;;) {
		PORTD = (1 << PORTD5);
		_delay_ms(1000);
		PORTD = (1 << PORTD6);
		_delay_ms(1000);
		PORTD = (1 << PORTD3);
		_delay_ms(1000);
	}
}
