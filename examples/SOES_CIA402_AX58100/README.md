# CiA 402 dummy servo drive

## Using AX58100 EtherCAT board with STM32 over SPI

This project is CiA 402 csp dummy on STM32F405x. Dummy as in loopback: does nothing, received target position is reported as actual position. CiA 402 is implemented, SOES works in mixed mode with DC sync.

The stuff I am doing aims at STMBL rather than ODrive (hence I started with oldschool StdPeriph libs), but both projects use STM32F405. One EtherCAT codebase done properly should be good on both with just the integration.

I have been working on that for some time now, and this is where I got:
- software licensing prohibits us from using Beckhoff SSC for open source, it leaves us SOES with GPL. For code generation tool, one can purchase EtherCAT SDK from rt-labs.
- hardware (boards designed by me) seems to be working. Slave nodes are reaching OP on both LAN9252 and AX58100
- measured performance shows that we better focus on AX58100. In freerun, with STM32F40x SPI1 at max speed, ecat_slv() loop takes < 20 us so we can easily get linuxcnc 5khz. LAN9252 sometimes took over 1000 us, and building (and debugging) DMA for its indexed PDI is going to be mess. 
Just check out https://github.com/OpenEtherCATsociety/SOES/blob/master/soes/hal/rt-kernel-twrk60/esc_hw.c (ET1100 code) vs https://github.com/OpenEtherCATsociety/SOES/blob/master/soes/hal/linux-lan9252/esc_hw.c (lan9252)
- CiA 402 is implemented and works with TwinCAT NC, SOES works in mixed mode with DC sync

## Project layout 

- `/esi_ref` contains relevant ESI file examples from misc sources
- `/lib/cia402` is CiA 402 servodrive state machine implementation, indepenent from used CANopen implementation
- `/lib/soes` is copied OpenEthercatSociety/SOES/soes content for easier future upgradeability.
- `/lib/soes/hal/ax58100` has added HAL for AX58100 over SPI
- `/lib/soes_hal_bsp` contains PDI drivers (for STM32F4 SPI with StdPeriph)
- `/lib/soes-esi` contains CoE Object Dictionary (OD), and EtherCAT Slave Info (ESI)
- `/lib/soes-esi/ax58100_patched` contains ESI files prepared for ASIX 58100 ESC by running `patch_esi.py` script


## Hardware

This projec uses AX58100 - SPI breakout rev 1

### AX58100 board pinout

| SPI1 | STM32 pin | AX58100 | AX pin | 
| ---- |:---------:|:-------:|:------:|
| MOSI |  PA7      | SMOSI   |   78   |
| MISO |  PA6      | SMISO   |   6    |
| SCK  |  PA5      | SCLK    |   64   |
| *    |  PA4      | SCS_ESC |   79   |
| *    |  PA3      | SINT    |   1    |
| *    |  PA2      | nRST    |   57   |
| *    |  PA1      | SYNC_L0 |   2    |
| *    |  PA0      | SYNC_L1 |   3    |

### ESC configuration

There are differences between AX58100 and ET1100 when it comes to configuration. As per datasheet:

- SCS_FUNC: Pullup high with 4K7 resistor is required for SPI operation

- Config Data: run `python3 .\patch_esi.py` to patch ESI files (prepared to ET1100) to work with AX58100


# STATUS

With ESI patched, and EVB port 0 connected to host :

- Writing .bin to EEPROM works. After disconnecting and reboot, reading EEPROM outputs binary identical to input
- EEP_DONE is high - .bin seems to be OK
- After STM32 Discovery is connected, LED_ERR is low - no application error
- Slave with STM32 is reaching OP.
- Slave stays OP 30 min without issues
- CiA402 axis is detected by TwinCAT
- DC mode is supported with stack in mixed mode


# TODO

- unit tests for CiA402 profile
- CiA402 homing
- Nested ESI projects ++ HAL directories that can be selected on compile time with build flags
```c
build_flags = 
    -I ./lib/soes/hal/stm32-lan9252
    -I ./libs/esi/stm32-lan9252
```
- verify cause of `printf("%d", i);` error on d > 9 (probably malloc problem)
- setup SYNC0 or 1 + benchmark that; check for consistency
- setup SPI IRQ + benchmark that; check if it cycle times are more consistent this way


### DMA Double buffer mode

If it works, `spi_dma_prepare_transmission` could be called only once at startup for each buffer. Now its done before each dma start as TX buffer is shared between both PDOs

https://github.com/maksutik/SDR-/blob/75013f7bc05bd95e8a19f533c182579c29c17052/STM32-SDR/code/main/src/Init_DMA.c#L62

https://github.com/HERO-ly/RC_TR_Code/blob/5fd8242dd5f4f04c0bf6a5bb98732e0a664cbb3f/RC_TR_Code/ROBOT/BSP/usart3.c#L66

https://github.com/spfanlost/myMcu/blob/5219ef711696eeae52bd57e2efc3befdccc930f0/src/mcu/mcu_i2s.c#L139


# Benchmarks

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

### DMA SPI

TxPDO works at full SPI speed (41 MHz) no problem. RxPDO worked at full speed with blocking SPI. 
When switching to DMA, reducing CLK with prescaler >= 8 was necessary. ESC has minimal read T, that is break between PDI address + read command bytes, and following data bytes, at least 240 ns. SPL software control of SPI data block introduced sufficient delay, DMA did it back to back, without any break and it turned out to be too fast.
After switching to read with wait state byte, SPI with prescaler 4 (that is 21 MHz) seems to work. SPI speed bumped to max still glitches. Most likely it is because of ESC not fetching valid data from memory to SPI fast enough, but maybe SPI traces routing on current PCB is not reliable at high speed? Also ESC_init doesnt wait for EEPROM to load as it should 
Well, it still is under 20us and 21 MHz is max speed for SPI2 available on ODrive anyway so its going to be good enough.

# STMBL pinout

STM32 | AX58100 | 12p | 12p | AX58100 | STM32 
______|_________|_____|_____|_________|______
                |+24V | +5V |
                | GND | SCK | SINT    | SWD SCK
 PD0  |  SYNC0  | CRX | GND |
 PD1  |  SCS    | CTX |SWDIO| SYNC1   | SWD IO
 PB4  |  MISO   | MISO| NRST|
 PB5  |  MOSI   | MOSI| SCK | SCLK    | PB3

