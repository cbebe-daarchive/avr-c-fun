#include <avr/io.h>
#include <util/delay.h>

#define BLINK_MS 150
#define LED PORTB5
#define LED_PORT PORTB
#define LED_DDR DDRB

#define BV(x) (1 << x)
#define setBit(P, B) P |= BV(B)
#define clearBit(P, B) P &= ~BV(B)
#define toggleBit(P, B) P ^= BV(B)

int main() {
    setBit(LED_DDR, LED);
    while (1) {
        toggleBit(LED_PORT, LED);
        // uses polling; busy waiting, not very efficient
        _delay_ms(BLINK_MS);
    }
}
