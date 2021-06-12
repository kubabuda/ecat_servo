# What is measured

Polled `ecat_slv()`, results are spewn on serial port. No interrupts, should be more deterministic and consistent

STM32F407 at 168 MHz, SPI1 at 42 MHz

# LAN9252

- Cables connected, ECAT master not connected: `[ESC benchmark] 0028 us (028 top)`
- ECAT master connected, slv in OP: `[ESC benchmark] 0072 us (0280 top)`