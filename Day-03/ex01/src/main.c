#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define RED rgb(true, false, false)
#define GREEN rgb(false, true, false)
#define BLUE rgb(false, false, true)
#define YELLOW rgb(true, true, false)
#define CYAN rgb(false, true, true)
#define MAGENTA rgb(true, false, true)
#define WHITE rgb(true, true, true)

void rgb(bool red, bool green, bool blue) {
	PORTD = 0;
	PORTD |= (red << PORTD5);
	PORTD |= (green << PORTD6);
	PORTD |= (blue << PORTD3);
	_delay_ms(1000);
}

int main() {
	DDRD |= (1 << DDD3) | (1 << DDD6) | (1 << DDD5);
	for (;;) {
		RED;
		GREEN;
		BLUE;
		YELLOW;
		CYAN;
		MAGENTA;
		WHITE;
	}
}
