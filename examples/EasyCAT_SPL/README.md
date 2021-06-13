# EasyCAT on STM32F4

# Using EasyCAT (LAN9252 EtherCAT board with SPI) library without Arduino

This is EasyCAT project with PDI code (SPI reads/writes to LAN9252 ESC) using StdPeriph drivers instead of Arduino

## MCU pinout

### Ecat devkit F407ZG

Devkit board uses SPI1 default pinout:

| SPI1 | STM32 pin | LAN9252 |
| ---- |:---------:|:-------:|
| CSN  |  PA4      | D5      |
| SCK  |  PA5      | D9      |
| MISO |  PA6      | D1/SIO1 |
| MOSI |  PA7      | D0/SIO0 |

### STMBL

STMBL is using SPI1 alternative functions

| SPI1 | STM32 pin | LAN9252 |
| ---- |:---------:|:-------:|
| *    |  PD0/PD1  | D5      |
| SCK  |  PB3      | D5      |
| MISO |  PB4      | D9      |
| MOSI |  PB5      | D1/SIO1 |

* Hardware CS is not exposed, you can use PD0, PD1 (CAN_RX, CAN_TX) for that

# MISC 🤷

- on STM32F407xx PE8 is used by EasyCAT as CS by default as it is mapped to Arduino digital pin 9 (default value for software SCS). To use PA4, HW NSS, pass it to EasyCAT constructor

- Seems like EasyCATs EasyConfigurator does sort variables in PDOs by size from biggest to smallest as it pleases, regardless of variables order on UI. This is probably to pack variables and minimize PDO size overall but for matching exact order of variables reordering in .h, .xml + generating .bin from .xml will be needed

- LAN9252 is picky when it comes to power supply. Its glitching without separate 3.3V regulator and Nano 5V line seems to be too weak for powering that regulator and few LEDs ¯\_(ツ)_/¯

- Removed SPI transaction in each MainTask, will fail if SPI1 is used for something else

- reading one indirect access register takes ~ 25us, in loop slave needs to read 2. Not great.

### Debug configuration

- Devkit board without reset line exposed, cannot use F4 Disco which is setting `reset_config srst_only` so cloned .json definition and changed `openocd_target`

# Benchmarking 

Measured is PDI communication cycle time (`EASYCAT.MainTask();`) on CiA DS402 PDOs. That is 6B each - RxPDO { uint16t; int32_t }; RxPDO { uint16t; int32_t }

| ESC     | SSC       | MCU     | SPI driver | SPI speed | value [us] |
| ------- | --------- |:-------:|:----------:|:---------:|:----------:|
| LAN9252 | EasyCAT   | AtM328P |  Arduino   | 8000000   | 196        |
| LAN9252 | EasyCAT   | STM32F4 |  Arduino   | 8000000   | 210        |
| LAN9252 | EasyCAT   | STM32F4 |  Arduino   | 42000000  | 118        |
| LAN9252 | EasyCAT.c | STM32F4 |  Arduino   | 42000000  | 107        |
| LAN9252 | EasyCAT   | STM32F4 |  SPL       | 5250000   | 123        | SPI prescaler 16
| LAN9252 | EasyCAT   | STM32F4 |  SPL       | 42000000  | 35         | SPI prescaler 2
