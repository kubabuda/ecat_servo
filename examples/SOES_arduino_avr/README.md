# SOES with PlatformIO

## Using LAN9252 EtherCAT board over SPI with AVR

This is SOES project compiled for Arduino board with ATMega (AVR) microcontroller. First version of SOES was originally targetting ATmega1281 (8 KB SRAM)
CoE data structures eat about 7 KB of RAM. Without wild hacks I dont see practical way to shrink it to under 2KB so unfortunately Arduino UNO (ATMega328) is out of question. Arduino Mega (ATMega 2560) has enough memory, but few changes were needed

## Changes to STM32 project:
- `esi/cc.h` uses atomic macros from `<util/atomic.h>`, not `<sys/param.h>`
- clean `esc_cfg_t config` initialization: all fields need to be referenced in initialization to avoid error `sorry, unimplemented: non-trivial designated initializers not supported` 

### TODO
- test on actual hardware: Arduino Mega with ATMega2560
- in `soes/hal` for AVR project, Arduino `SPI.h` header is not available. avr-g++ is older compiler, maybe nested setup (CPP library SPI in C library SOES in CPP project) is not available. Instead of diving into pio internals, just declare and use SPI registers
- resolve build warnings

## MCU pinout

### Arduino Mega

| SPI  | Arduino | LAN9252 |
| ---- |:-------:|:-------:|
| CSN  |  9      | D5      |
| SCK  |  13     | D9      |
| MISO |  12     | D1/SIO1 |
| MOSI |  11     | D0/SIO0 |

# Status

- AVR Arduino - compiles, eats 7KB RAM, SPI driver is not connected
Remember to use levels shifter between AVR and LAN252.

# Results - benchmarks - TODO

Measured is PDI communication cycle time (`EASYCAT.MainTask();`) on CiA DS402 PDOs. That is 6B each - RxPDO { uint16t; int32_t }; RxPDO { uint16t; int32_t }

| ESC     | SSC       | MCU     | SPI driver | SPI speed | value [us] |
| ------- | --------- |:-------:|:----------:|:---------:|:----------:|
| LAN9252 | SOES      | AtM328P |  Arduino   | 8000000   | [TODO]     |
