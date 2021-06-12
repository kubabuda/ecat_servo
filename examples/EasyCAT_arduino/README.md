# Using EasyCAT (LAN9252 EtherCAT board with SPI) with PlatformIO

## MCU pinout
### ecat devkit F407ZG
Devkit board uses SPI1 default pinout:

| SPI1 | STM32 pin | LAN9252 |
| ---- |:---------:|:-------:|
| CSN  |  PA4      | D5      |
| SCK  |  PA5      | D9      |
| MISO |  PA6      | D1/SIO1 |
| MOSI |  PA7      | D0/SIO0 |

### Arduino Nano

| SPI  | Arduino | LAN9252 |
| ---- |:-------:|:-------:|
| CSN  |  9      | D5      |
| SCK  |  13     | D9      |
| MISO |  12     | D1/SIO1 |
| MOSI |  11     | D0/SIO0 |

# Status

- AVR Arduino - everything works just fine (even tho ATmega was working on 5V and LAN9252 is not 5V tolerant, 3.6V max)
- port to STM32Duino and launch on STM32F407Z-LAN9252 devkit. Configuring, TxPDOs are working with simple explorer

# Results - benchmarks

Measured is PDI communication cycle time (`EASYCAT.MainTask();`) on CiA DS402 PDOs. That is 6B each - RxPDO { uint16t; int32_t }; RxPDO { uint16t; int32_t }

| ESC     | SSC       | MCU     | SPI driver | SPI speed | value [us] |
| ------- | --------- |:-------:|:----------:|:---------:|:----------:|
| LAN9252 | EasyCAT   | AtM328P |  Arduino   | 8000000   | 196        |
| LAN9252 | EasyCAT   | STM32F4 |  Arduino   | 8000000   | 210        |
| LAN9252 | EasyCAT   | STM32F4 |  Arduino   | 42000000  | 118        |
| LAN9252 | EasyCAT.c | STM32F4 |  Arduino   | 42000000  | 107        |