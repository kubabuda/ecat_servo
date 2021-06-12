# Benchmarking 

Data format on all benchmarks is CiA DS402 PDOs. That is 6B each - RxPDO { uint16t; int32_t }; RxPDO { uint16t; int32_t }

## EasyCAT

Measured is PDI communication cycle time (`EASYCAT.MainTask();`) from start to return.

| ESC     | SSC       | MCU     | SPI driver | SPI speed | value [us] |
| ------- | --------- |:-------:|:----------:|:---------:|:----------:|
| LAN9252 | EasyCAT   | AtM328P |  Arduino   | 8000000   | 196        |
| LAN9252 | EasyCAT   | STM32F4 |  Arduino   | 8000000   | 210        |
| LAN9252 | EasyCAT   | STM32F4 |  Arduino   | 42000000  | 118        |
| LAN9252 | EasyCAT   | STM32F4 |  Arduino   | 42000000  | 107        |
| LAN9252 | EasyCAT   | STM32F4 |  SPL       | 42000000  | 123        | SPI prescaler 16
| LAN9252 | EasyCAT   | STM32F4 |  SPL       | 42000000  | 35         | SPI prescaler 2

## SOES

Measured is how long does polled `ecat_slv();` take from start to return. No interrupts, should be more deterministic and consistent. 

### LAN9252

STM32F405 at 168 MHz, SPI1 at 42 MHz, SPL driver

- Cables connected, ECAT master not connected: `[ESC benchmark] 0028 us (028 top)`
- ECAT master connected, slv in OP: `[ESC benchmark] 0072 us (0280 top)`

STM32F103 SPI1 at 18 MHz, Arduino driver

- Cables connected, ECAT master not connected: `[ESC benchmark] 226 us (391 top)`
- ECAT master connected, slv in OP: `[ESC benchmark] 581 us (2393 top) [ESC benchmark] 943 us (2393 top)`

### AX58100

Polling (blocking SPI). SPI1 at 42 MHz

- Cables connected, ECAT master not connected: `[ESC benchmark] 0006 us (0006 top)`
- ECAT master connected, slv in OP: `[ESC benchmark] 0015 us (0059 top)`
- ECAT master connected, slv in OP, network startup hiscore reset: `[ESC benchmark] 0015 us (0024 top)`

Added CiA402 loopback, SPI1 at 42 MHz:

- ECAT master connected, slv in OP, network startup hiscore reset: `[ESC benchmark] 0017 us (0025 top)`

DMA SPI + CiA402 loopback. Prescaler 2 (42 MHz), SPI reads from ESC are **unstable - transmission errors**

- ECAT master connected, slv in OP: `[ESC benchmark] 0017 us (0063 top)`
- ECAT master connected, slv in OP, network startup hiscore reset: `[ESC benchmark] 0017 us (0025 top)`

DMA SPI + CiA402 loopback. Prescaler 4 (21 MHz: 2x slower clock), **2 us slower**

- Cables connected, ECAT master not connected: `[ESC benchmark] 0009 us (0009 top)`
- ECAT master connected, slv in OP: `[ESC benchmark] 0019 us (0073 top)`
- ECAT master connected, slv in OP, network startup hiscore reset: `[ESC benchmark] 0019 us (0030 top)`
