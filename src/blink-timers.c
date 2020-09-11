#include <avr/interrupt.h>
#include <avr/io.h>

// defines for bit twiddling
#define BV(x) (1 << x)
#define setBit(P, B) P |= BV(B)
#define clearBit(P, B) P &= ~BV(B)
#define toggleBit(P, B) P ^= BV(B)

#define LED PORTB5
#define LED_PORT PORTB
#define LED_DDR DDRB

#define PRESCALER 10  // 10 bits... 1024
#define CLOCK_RATE 15998976
#define COUNTER_VALUE (CLOCK_RATE >> PRESCALER)

// TIMER1 compare interrupt service routine
// fires when counter value matches OCR1A
ISR(TIMER1_COMPA_vect) { toggleBit(LED_PORT, LED); }

int main() {
    setBit(LED_DDR, LED);
    // OCR1A = output compare register for Timer 1
    OCR1A = COUNTER_VALUE;

    // 16 MHz clock becomes about 16kHz

    // Timer counter configuration register for Timer 1
    TCCR1A = 0x00;
    setBit(TCCR1B, WGM12);          // CTC Mode - compare to OCR1A
    TCCR1B |= BV(CS10) | BV(CS12);  // Set Prescaler to 1024

    // get an interrupt
    setBit(TIMSK1, OCIE1A);

    // enable interrupts
    sei();

    while (1) {
        // do something useful in here while we are waiting
        // for the timer to fire
    }
}
