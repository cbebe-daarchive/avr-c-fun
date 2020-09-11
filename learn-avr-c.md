# Learning AVR-C

## Special registers

These two registers change the direction and state of the pins
DDRx - direction registers (1 - output, 0 - input)
PORTx - state registers (1 - on, 0 - off)

For example, if we want to write on pin PB0, we set the 0th bit of DDRB to 1 (output)
then toggle the 0th bit on PORTB (1 - on, 0 - off)