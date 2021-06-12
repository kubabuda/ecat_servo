# SOES on STM32

This is example project using SOES with LAN9252 and STM32F4

## LAN9252 EtherCAT board with STM32F4 over SPI

## MCU pinout

Discovery F407 adapter pinout

| SPI1 | STM32 pin | LAN9252 |
| ---- |:---------:|:-------:|
| *    |  PD0/PD1  | D5      |
| SCK  |  PB3      | D5      |
| MISO |  PB4      | D9      |
| MOSI |  PB5      | D1/SIO1 |

STMBL is using SPI1 alternative functions. Hardware CS is not exposed, you can use PD0, PD1 (CAN_RX, CAN_TX) for that

# Benchmarks

What is measured: polled `ecat_slv()`, results are spewn on serial port. No interrupts, should be more deterministic and consistent. STM32F407 at 168 MHz, SPI1 at 42 MHz

- Cables connected, ECAT master not connected: `[ESC benchmark] 0028 us (028 top)`
- ECAT master connected, slv in OP: `[ESC benchmark] 0072 us (0280 top)`
