# Working with LinuxCNC

LinuxCNC does not support Ethercat natively, probably due to licensing reasons. To get [EtherCAT driver](https://github.com/aschiffler/linuxcnc/) you have to build it and setup with IgH EtherLab that functions as master. To make life easier, prebuilt [OS image](https://github.com/grotius-cnc/LINUX_RTOS) can be used.

Configuration:

Looks like dummy servodrive is working. Time to configure Distributed Clock. We can either set `0x0980` to 0x01, to assign sync unit to PDI (and let device slave finish configuration), or do all the sync configuration on master side.

[TODO] link configuration

[CiA402 HAL component](https://github.com/dbraun1981/hal-cia402)



[Next: AX58100 rev 2, STMBL adapter](https://kubabuda.github.io/ecat_servo/008-ax58100-board-rev2)

[Prev: AX58100 board](https://kubabuda.github.io/ecat_servo/006-ax58100-board)

[Back to the table of contents](https://kubabuda.github.io/ecat_servo)
