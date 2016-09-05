avr-aes256enc
=============

05-Sep-16  Markku-Juhani O. Saarinen <mjos@iki.fi>

A public domain constant-time implementation of **AES-256** on 8-bit 
*AVR microcontroller*, just because "you never know when you might need one" !

We implement block encryption only -- to get an encryption/decryption thing 
going, use CTR or similar. Note that inverse AES is much slower and cannot
reuse the same components.

The actual assembly language implementation is in `aes256enc.S`, the
rest is just testing.

# Running on Arduino

The flashing code is incorporated into Makefile. You need to install 
(just `apt install` on Debian/Ubuntu) `arduino` and `avrdude` packages.

Plug Arduino into a USB port and do a `make flash`. Now the thing will compile 
code, flash it, and dump output via UART with stty magic to your standard 
output. Most relevant settings are in `Makefile` -- you should edit that
first if something fails. The default settings with my stock Arduino Uno (R3) 
with Ubuntu 16.04.

```
c$ make flash
mkdir -p obj/
avr-gcc -Wall -Ofast -mmcu=atmega328p -DF_CPU=16000000 -c aes256enc.S -o obj/aes256enc.o
```
.. more stuff.. and then the interesting bit:
```
AES-256 Self-Test: OK
Run #00 5578 ticks / block
Run #01 5567 ticks / block
Run #02 5562 ticks / block
Run #03 5551 ticks / block
Run #04 5557 ticks / block
Run #05 5557 ticks / block
Run #06 5556 ticks / block
Run #07 5556 ticks / block
Run #08 5556 ticks / block
Run #09 5556 ticks / block
```
That's a success!

# Running on SIMAVR

If you don't have an Arduino lying about, you can fetch
`simavr` (https://github.com/buserror/simavr) and test using it with
`make sim`.

This cycle-perfect simulator simulates even the UART so the output will be
basically equivalent to above:
```
$ make sim
simavr -v -v -v -m atmega328p avr-aes256enc
Loaded 3914 .text at address 0x0
Loaded 566 .data
UART: 0 configured to 0019 = 2403 bps (x1), 8 data 1 stop
UART: Roughly 4587 usec per bytes
AES-256 Self-Test: OK.
TIMER: avr_timer_configure-0 TOP 3906.25Hz = 256 cycles = 256usec
TIMER: avr_timer_configure-1 TOP 0.06Hz = 16778290 cycles = 16778290usec
Run #00 5578 ticks / block.
Run #01 5567 ticks / block.
Run #02 5546 ticks / block.
Run #03 5559 ticks / block.
Run #04 5561 ticks / block.
Run #05 5557 ticks / block.
Run #06 5555 ticks / block.
Run #07 5555 ticks / block.
Run #08 5555 ticks / block.
Run #09 5555 ticks / block.
^Csignal caught, simavr terminating
```

Have fun.

Cheers, -markku

**ABSOLUTELY NO WARRANTY WHATSOEVER**

