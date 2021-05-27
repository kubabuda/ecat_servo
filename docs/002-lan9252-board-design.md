# LAN9252
## SPI adapter board design

Starting point was this projectL https://github.com/yuqlid/EtherCAT_shield_v1 . Stripped schematic to bare minimum, based on EVB-LAN9252-SPI. Left only SQI PDI, then minified board area. Two layers, almost all components on single side for easier assembly and embedding into target devices.

![lan9252spi_rev1_top](img/lan9252rev1_top.png "LAN9252-SPI rev 1 render top")

![lan9252spi_rev1_bottom](img/lan9252rev1_bottom.png "LAN9252-SPI rev 1 render bottom")

Adapter board, first revision, straight from KiCad.

[TODO] link img/lan9252rev1_ibom.html

Looks good. Board came out similar to EasyCAT Pro, breakout header is compatible, but additionally has full SQI exposed. Also EEPROM chip is THT in socket, for simple swap if ESI flashing goes wrong and bricks device. Now forward to board house of choice, and wait for PCBs to arrive.