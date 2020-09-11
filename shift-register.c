/**
 * shift-register.c
 * Learning how to use the 74HC595 8-bit shift register
 * Code example from: https://embedds.com/interfacing-shift-register-with-avr/
 */

#include <avr/io.h>
#include <util/delay.h>

// defines for bit twiddling
#define BV(x) (1 << x)
#define setBit(P, B) P |= BV(B)
#define clearBit(P, B) P &= ~BV(B)
#define toggleBit(P, B) P ^= BV(B)

#define HC595_PORT PORTB
#define HC595_DDR DDRB
#define HC595_DS_POS PB0     // data pin
#define HC595_SH_CP_POS PB1  // shift clock pulse pin
#define HC595_ST_CP_POS PB2  // store clock pulse pin

void shiftInit() {
    // initialize HC595 pins
    HC595_DDR |= BV(HC595_DS_POS) | BV(HC595_SH_CP_POS) | BV(HC595_ST_CP_POS);
}

// change data pin output
#define HC595DataHigh() setBit(HC595_PORT, HC595_DS_POS)
#define HC595DataLow() clearBit(HC595_PORT, HC595_DS_POS)

// send a clock pulse on sh_cp line
void shiftPulse() {
    // pulse the shift clock
    setBit(HC595_PORT, HC595_SH_CP_POS);
    clearBit(HC595_PORT, HC595_SH_CP_POS);
}

// send a clock pulse on st_cp line
void shiftLatch() {
    setBit(HC595_PORT, HC595_ST_CP_POS);
    _delay_loop_1(1);
    clearBit(HC595_PORT, HC595_ST_CP_POS);
    _delay_loop_1(1);
}

/*
Main High level function to write a single byte to
Output shift register 74HC595.
Arguments:
   single byte to write to the 74HC595 IC
Returns:
   NONE
Description:
   The byte is serially transferred to 74HC595
   and then latched. The byte is then available on
   output line Q0 to Q7 of the HC595 IC.
*/

// send each bit serially with MSB first
void shiftWrite(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        // get MSB
        if (data & 0x80)
            HC595DataHigh();
        else
            HC595DataLow();

        // shift the data by 1 bit
        shiftPulse();
        data <<= 1;
    }
    // store the data in the chip
    shiftLatch();
}

#define PRESCALER 10  // 10 bits... 1024
#define CLOCK_RATE 15998976
#define COUNTER_VALUE (CLOCK_RATE >> PRESCALER)

static uint8_t counter = 0;

// TIMER1 compare interrupt service routine
// fires when counter value matches OCR1A
ISR(TIMER1_COMPA_vect) {
    uint8_t led[8] = {0b10101010, 0b01010101, 0b00110011, 0b11001100,
                      0b11110000, 0b00001111, 0b11111111, 0b00000000};
    shiftWrite(led[counter++]);

    if (counter >= 8) counter = 0;
}

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
