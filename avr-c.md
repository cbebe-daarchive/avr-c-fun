# Programming AVR Microcontrollers in C

From YouTube: [https://youtu.be/ERY7d7W-6nA]
## Quick overview

**The Familiar**

- Control external devices by connecting them to AVR pins
- Communicate with your desktop/laptop over serial
- PWM (alias analogWrite()), ADC (alias analogRead()), initialization/event loop structure

**The New**

- Different toolchain to program chip
- Learn a few microcontroller-C programming idioms
- Learn about, configure, eventually master the internal hardware peripherals

**Microcontroller-style C Pros**

- Speed: your code can run *much* faster
- Flexibility: make the chip do what you want/need
- Responsiveness: do many things at once, respond instantly
- Portability: C is available for every(?) CPU
- Portability II: your code will work on all AVRs
- Curiosity: just to learn more about microcontrollers


**Microcontrollers**

Pros
- Write programs in various languages (C, assembly, BASIC)
- CPU (1-20MHz)
- Dynamic Memory (SRAM)
- Non-volatile memory (Flash ROM and EEPROM)

Cons
- 8-bit words
- Not much memory (1-32KB program space, couple KB SRAM)
- No operating system
- Low-level input/output

Cool Stuff

- Super-fancy blinkers, POV toys
- Robots/Quadcopters/3D printers
- Dataloggers (GPS, Energy monitors)
- USB devices
- Interface between real world and computer world - glue logic in many devices

Output

- Digital output: Apply 0V or 5V to any pin

  - Light up LEDs, flip switches, spin motors
  - Digital communication: UART, SPI, I2C, USB

- Pulse-width Modulation (PWM) for "analog" output: 

  - Arduino's analogWrite()
  - Flip the digital output on and off quickly
  - Percentage of time high/low determines average voltage

Input - Read voltage levels applied to pins
  
- Digital (pushbuttons, threshold sensors), and of course receiving digital communications
- Analog-to-Digital Conversion (light levels, audio waveforms)

- Input Modes:
  - "Hi-Z" (high impedance): effectively disconnected from the circuit
  - Internal pullup resistor: test if anything connected

Making your life easier

- Most everything you can do with an AVR _can_ be done just by toggling pins (ADC is the big exception)
- But writing code to do send and receive serial data is miserable, and would tie up the CPU
- Robot: monitor the serial line, control (via PWM) motor speed, blink some LEDs, and be continually ready to turn of its killer laser when it detects a person.
- Doing multiple things at once - offload tasks to onboard hardware peripherals
- Learn the hardware

Short version

- The Arduino is a nice development kit
- It has everything you need
- It has more than you need sometimes
- The Wiring/Arduino "language" is very simple to learn.
- Too simple. You gloss over a lot of the good stuff
- "Working with Arduino is like knitting in boxing gloves."
- Forty years of microcontroller development has been aimed at making it easier for you to realize your projects

## The Toolchain

The Lifecycle of AVR Programming

- Write code in C (using whatever you want)
- Cross-compile for the chip -> the AVR machine-code version of your code
- Transfer code to the chip:
  - Hardware programmer to talk to the chip
  - Software to run the programmer
- Get feedback and debug until it works

What you'll need to download or buy

- Cross-compiler: GNU avr-gcc and associated software tools
- Hardware programmer (or a previously installed bootloader)
- AVRDUDE: knows how to run many hardware programmers
- Usually a Makefile to compile and flash for you in one step
- USB-serial converter: "printf" debugging feedback

Packages

- Linux: sudo apt-get install avrdude binutils-avr avr-libc avr-gcc
- Windows: WinAVR, or Atmel's AVR studio
- Mac: CrossPack (optionally XCode)

### Programming the chip

**On breadboard**

- AVR uses the SPI for In-System Programming (ISP)
- Bottom line is that you need to hook up four signal wires, plus power and ground
  - SCK (serial clock)
  - MISO (master-in, slave-out)
  - MOSI (master-out, slave-in)
  - RESET (tell the AVR to enter programming mode)
  - VCC, GND

**AVR Programming AVRs**

- In the Files...Examples menu, you'll find Arduino ISP
- Flash that in
- Now you're ready to talk to the AVR through the Arduino
- avrdude -p atmega168 -c avrisp -b 19200 -P /dev/ttyACM0 -nv
- If that works, you'll see a lot of details about the chip on your breadboard
- If it fails, recheck connections

**Using another Programmer:**

**DIY**

- Once you've got a working Arduino AVR programmer, you can make your own permanent ISP
- Search "VUSBTiny" for a truly minimal design
- Also see USBTiny and USBasp projects (DIY Versions)
- Parallel port connector and 5 wires (DAPA)

**Or Go Pro**

- AVRISP MkII (Atmel's Own. Very Robust)
- LadyAda, Sparkfun, etc sell USBTiny kits
- Bus Pirate (SPI mode)
- USBasp-based designs available for $5 form the far east

**... and Flash**

- First thing to do is tweak your Makefile so that it matches your setup
  - target chip (MCU)
  - programmer
  - Serial port and speed if necessary (programmer options)

- Make sure you're opening up the Makefile in the same directory as your project
- Open up a terminal window in the project directory
- Run make flash or make program and you're off to the races

- Just to be sure we know what's going on: 
  - Type make to create the AVR machine-code file
  - Check for errors, heed warnings
  - Your code needs to compile successfully first before you upload

- avrdude -p atmega168 -c usbtiny -U blinkLED.hex
- avrdude -p atmega168 -c avrisp -b 19200 -P /dev/ttyACM0 -U blinkLED.hex

**What you need to know**

- -p chip: what chip type are you trying to program?
- -c programmer: What programmer are you using?
- -U hexfile: Which file to upload?
- And some optional options
- It's good to be able to test these out by hand
- You'll want to personalize these values in your Makefile

## Peripherals and their Configuration

**Useful Built-in Hardware**

- Timers: AVR ATMegas have three internal timer/counters, useful for counting, timing, and scheduling events (same as millis())
- Timers also make PWM easy: Arduino "analog" pins
- Interrupts: Internally- or externally-triggered: Run code whenever an event happens
- Serial I/O: built-in hardware for USART, SPI, I2C serial protocols
- ADC: convert analog voltage to digital numbers
- EEPROM Memory: Read/Write memory that doesn't get lost when the power goes out

**... are Awesome**

- The ADC, serial, and timer/counter peripherals run independently of the CPU
- All of the peripherals can trigger interrupts; your code doesn't have to wait for incoming serial data, but can instead be interrupted only when a new byte comes in
- Clever use of these features enable your chip to do many things at once
- Each of these peripheral devices are very flexible
- (Arduino hides a lot of this from you)
- (erm... I mean, does a lot of this for you)

**... Require Configuration**

- You have a bunch of configuration to do
- I/O - select input or output mode, hi-z or pullup if input
- PWM: base timer clock speed, set PWM mode: toggle pin? trigger interrupt?

**Datasheet is maybe your Frenemy**

- Datasheet for ATMega48/88/168 is 660 pages long
- Encyclopedia, not novel
- Page one and two are a really good read
  - Page one: summary of peripherals, how much power the chip needs, its maximum voltage
  - Page two: pinout diagram that explains the functions of the different pins

- After that, skip to the chapters you need
- Read chapter intro, try to understand the block diagram
- Now you're ready to configure the Registers

**The Secret to Control**

- Registers ("special function registers") are fixed memory locations with side effects
- Read and write just like a normal variable
- Each register byte is bits - think of each bit as a switch
- Each switch has a side effect, depends on the register, which bit
- In blink.c, we wrote PORTB = 0b00000010;
- Setting this register's value flips the number 1 bit, turns on PB1

**Input/Output pins**

- Don't usually think of them as being "hardware peripherals" but even the I/O pins need configuration
- In Arduino, we are used to to calling pinMode() to get this done
- In C, write directly to the special function register that controls the pin's data direction, the Data Direction Register (DDR)
- Code: DDRB = 0b00000010 sets pin one in PORTB (PB1) into output mode
- Warning: The AVR hardware (and C) starts counting at 0
- Everything the chip does is configured by setting and clearing bits in registers

**Bits in registers**

- Let's set up PWM on PB1 (OC1A) to run at around 1kHz with no CPU involvement
- We need to configure three things:
  - choose a Timer clock source
  - set up PWM mode ("Fast PWMi, 10-bit mode") 
  - enable automatic output on PB1
  
**All that binary is miserable**

- To set 10-bit Fast PWM mode, we need to set bits WGM10, WGM11, WGM12
- So we could look up the bits in the register, find out that WGM10 is bit zero and WGM11 is bit one. We then assign TCCR1A = 3;, which is the sum of the two bits in binary
- Now we also need to enable output on PB1, so we look up that bit: COM1AO

## C for Microcontrollers

**C is not C**

- A lot of microcontroller programming is necessarily low-level, we've got no OS here, and we're flipping bits
- Microcontroller C can be ANSI C, but you're going to use a different part of it than you're used to
- C has provisions for doing bit-wise manipulations, bit shifts, bitwise AND and OR, etc.
- Enter Bit Twiddling: bit-shifting, bit-masking, bitwise logical operations

**Bit operations**

- Set bit with OR: x |= 1
- Toggle bit with XOR: x^= 1
- Clear bit with AND and NOT: x &= ~1

**There was a point to all this**

- Remember we had these bit names from the datasheet
- We wanted a way to set/clear bits in registers to control various peripherals
- Now we can set/clear them by name:
  - DDRB |= (1 << PB1); sets the "PB1"th bit in DDRB
- And we can set multiple bits:
  - TCCR1A |= ((1 << WGM10) | (1 << WGM11)| (1 << COM1A0));
- And if we wanted to clear the COM bit without changing the mode bits:
  - TCCR1A &= ~(1 << COM1A0);

**That was hard!**

- It was. You'll get used to it.
- Alternative:
```
#define BV(x) (1<< x)
#define setBit(P, B) (P |= BV(B))
#define clearBit(P, B) (P &= ~BV(B))
#define toggleBit(P, B) (P ^= BV(B))
```
- Macro versions are clearer, easier to read, but other people will use the basic form so you need to understand it

## Wrap-up and Resources

**Covered a lot of Ground**

- The AVR-GCC programming toolchain
- Million-mile overview to the important AVR peripherals and why they matter
- Rudimentary bit-twiddling and how it works with configuration registers

**What's left?**

- The specifics of peripherals: how they work, how they help
- Setting up event priorities with interrupts
- Watchdog timer, sleep modes for power saving, etc.

**Some Links**

- AVR Site: [www.littlehacks.org]
- Old material from AVR Classes: [https://wiki.hacdc.org/index.php?title=AVR_Microcontroller_Class_2011]
- Bruce Land's Cornell University Engineering Course (for the serious down low)