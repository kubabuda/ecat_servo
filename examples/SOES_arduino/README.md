# SOES as Arduino library

This project shows using SOES library with Arduino (STM32Duino) on ubiqutous STM32F103 "BluePill" board. 

- ESC is LAN9252 connected over SPI
- example application is CiA 402 dummy servo drive.


## MCU pinout

STM32F103 BluePill 

| SPI1 | STM32 pin |  PDI | LAN9252 |
| ---- |:---------:|:----:|:-------:|
| *    |  PA4      | SCS  | D5      |
| SCK  |  PA5      | SCK  | D5      |
| MISO |  PA6      | MISO | D9      |
| MOSI |  PA7      | MOSI | D1/SIO1 |
| UART1| 
| TX   | PA09
| RX   | PA10

# Benchmark

- Cables connected, ECAT master not connected: `[ESC benchmark] 226 us (391 top)`
- ECAT master connected, slv in OP: `[ESC benchmark] 581 us (2393 top) [ESC benchmark] 943 us (2393 top)`

Good enough for sensor or dummy servodrive, looks like not enough for real life motion control usage.