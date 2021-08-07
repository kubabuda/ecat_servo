# EtherCAT compatible servo drives for everyone

I am building EtherCAT adapter for open motor controllers like ODrive, STMBL, or your next thing.

# Why that matters

We got a known problem in state of art open source robotics. On one side for years we got things like LinuxCNC: multiple axis, builtin PLC, free, field proven. If its not fit for application, every year progress in development tools (toolchains, code etitors, hardware) makes it easier to roll out custom one that will do the job. On he other side, we got plenty of OSS+OSHW actuators: projects like ODrive, STMBL, VESC and many more. Good to power things from BLDC motor in quadcopter, to benchtop laser ploter, to electric scooter, to industrial machining center. What is missing? Good free way to properly connect one to another. This might not sound like a problem for electric scooter, but starts to be when one tries to build robotic arm, quadruped robot or fast CNC machine, and I will explain why.

## How OSS motion control used to be done

Legacy way of motion control in open source was to use step-dir over parallel port. It is simple way to control stepper motors working in open loop (without encoders or other sensors), devised back when everyone used PC towers that had LPT. Some modern digital servodrives are still supporting it, but this starts to be problematic for several reasons.
- This is in principle open loop communication. Lost pulse signal will result in quiet position error, as controller computer will not have a way to see target drive position. It is especially not elegant in using step-dir for servo drives position control. Position value is digital on controller side and on servodriver side, but communication channel makes it drop its digital nature, then pick it up again, then prevents actual position feedback to controller. Uuugly. 
- It is speed bottleneck: speed of software step generation is limited. Exceeding it will result in errors: lost steps. Depending on PC setup, pushing pulse rates above 20 KHz - 100 KHz starts to be ridden with error. Compose it with open loop, and you only see this errors if it accumulates and becomes real problem (e.g. CNC machine plunges into workpiece few hours in the job).
- Additional I/O (homing, touchprobes, interlocks, signal lights) is limited. Parallel port lines can also be used for IO but they are fixed: both in number (there is only 16 lines total, les than half can be used as inputs), and position (some lines cannot be uset as inputs).
- Self diagnostics is limited: servo drive might detect and identify error state, but has no way to sent its code to controller.  

## How it is done now (in open source and entry level motion controllers)

### LinuxCNC

- Linux CNC with LPT still is viable solution for machines with few stepper motors and limited IO. All drawbacks described above apply, but for CNC router this often is enough
- Linux CNC with Mesa cards: Mesa is company that offers line of extension cards with FPGA, targeting LinuxCNC. Bitfiles source code is open, hardware is proprietary but not expesive. There are multiple connectivity configurations for each card. First is using hardware step signal generation to overcome software stepgen limits. Much more reliable, works all right with servo drives supporting pulse-dir signals. Some cards got analog inputs and outputs and can be used to control older, voltage controlled servo amplifiers but this is really legacy solution for retrofits.
- There is also smart serial (sserial), Mesa specific protocol for modular extensions. It uses RS422 differential transmission for fast, noise resistant communication. If you want to use STMBL servo drives to build CNC machine, this seems to be the preffered way. Problem is STMBLs are hard to buy, and those (and some Mesa servo drives) are the only devices using sserial protocol (that I know of).
- LinuxCNC with proprietary extension cards, from vendors like yurtaev.com. These are adding support for proprietary protocols like SSCNET, MELDAS, Mechatrolink. Cards themselves are not open source, pricing is "contact us". There is no open servodrive project using these. Drives from industrial vendors like Yaskawa or Mitsubishi will do its job for years, at industrial machine prices. 
- LCEC: LinuxCNC with EtherCAT. Software is free. Fast, modular, requires no special hardware on controller side. Generic driver probably will work on computer you already have, available drivers support commodity PCI network cards available for few dollars. This protocol seems to be industrial standard gaining more and more popularity. Servo drives and I/O are available from plenty of industrial vendors. No open source actuators ... so far.

### Microcontroller based motion controllers

- GRBL, GRBL ports to 32 bit MCUs (ARM, ESP32) and other 32bit MCU CNC controller projects like Smoothieboard. Using USB or Ethernet to connect to desktop running UI, outputing pulse-dir signals for stepper motors. Well known from being used in 3D printers, usually equipped with stepper motor drivers rated for small NEMA 17 sized motors, could work just as well for CNC router (when connected to external stepper drivers). Original 8bit GRBL was limited to 3 axis, its ports and competing project can support up to 6 axis. Pulse-dir signal generation should be faster and more stable than on desktop LPT, rest of limitations is similar.

### Mach

Mach is not open source, but is affordable PC based controller with large entry level user base

- Mach 3 / Mach 4 controller using LPT port, limitations to that are described above. 
- Mach 3 / Mach 4 with hardware controller: CSMIO, SmoothStepper. This adds hadrware pulse-dir signal generation for more reliability and enough I/O for most CNC applications.
- Mach 4 with EtherCAT driver: several vendors are offering various solutions.

### CAN bus

- Application specific protocol over CAN bus. CAN transcievers are cheap, MCUs like STM32F405 used in ODrive got CAN controllers builtin, protocol uses noise resistant transmission over differential pairs. At top speed it is fast enough for few axis at hundreds of Hz; bandwidth does not leave much room for more axis, faster control loops, or additional I/O. Cheap way but non standardized at all: seems like quadruped robot actuators have their data formats, quadcopters their own, and so on. Drawbacks: Linux supports CAN, but AFAIK are there no realtime CAN adapters for Linux available that are not in $$$s range. Cheaper options are USB to CAN adapters like CANtacts, CANable, but USB is not good for realtime required for motion control. Some projects are solving this by running realtime part on  This adds another non standard layer

- CAN bus with CANopen devices. CANopen is standarization of data format for various devices commuincating over CAN bus. When it comes to motion control, device profile for servo drive over CAN bus is called CiA (CAN in Automation) 402; industrial servodrives implementing this profile are available from number of vendors. This approach would still have some of the drawbacks of using custom CAN protocols (no affordable realtime CAN adapter for PC controller, limited speed and bandwidth in comparison to Ethernet based protocols). Apparently this is not popular solution in open source, no wonder.

### Summary

I am sure I ommited some motion control solutions from this market segment. Let me know if I need to correct something.

# How it could be done

We need simple to use, low cost and powerful solution that is compatible with industry standards, that is not totally owned by That One Company Behind It. And we need to make it completely open source, so we can do with it what we want. From all the options above, solution that is nearest to this all is EtherCAT. It gives us fast controller on the cheap, plenty of I/O and actuators to choose from numerous vendors, is properly supported with LinuxCNC, has multiple open source master libraries (SOEM, EtherLab) and more. 

# What is there to be done

## Licensing

EtherCAT Technology Group (ETG) calls it open network, but it is not open as in GPL or MIT license. It means anyone can sign up to be member of ETG and get access to documentation, code samples and code generation tools, for free. It comes however with obligation not disclose any of these materials to anyone who is not member of ETG. 
EtherCAT trademark licensing implies also that no device can be sold with EtherCAT markings if vendor offering it has no valid Conformance Testing Tool license.
Official code stack from Beckhoff is called Slave Stack Code. Licensing on that code prohibits it from being used in open source projects. Luckily there is alternative, GPL licensed stack: SOES from Open EtherCAT Society. This means following project is going to be under GPL too.

TLDR one should be good with using EtherCAT in OSS project if he use SOES, and does not sell anything marked with EtherCAT logo or name

## Select motion control protocol

EtherCAT itself is only data link layer. One could roll out his own protocol on top of it, but what we want is to adapt standard protocols for interoperability with devices available on the market. Most popular protocol for EtherCAT is CoE: CANopen over EtherCAT, that mirrors CANopen way of working with Object Dictionary and Protocol Data Objects, but uses EtherCAT network instead of CAN bus. For motion control over EtherCAT, it means CoE CiA402. There is alternative protocol, SERCOS over EtherCAT, which seems to be way less popular and (for new, open source projects) has no real edge over CoE CiA402 that I know of.

This project will focus on CiA402, and will start by implementing its csp mode (cyclic synchronous position), as most convinient to use with CNC controllers like LinuxCNC.

## Get hardware

### Select ESC (Ethercat Slave Controller) chip

EtherCAT does not require special hardware on controller (master) side, generic Ethernet adapter is all that is needed. On slave side, hardware support is neecessary in form of dedicated ESC chip. Well, it can be also done with FPGA and IP cores are available for purchase but I am going to leave this aside for now. 

- Original ESC is ET1100 from Beckhoff. It exposes 32 configurable GPIO for simple use cases like distributed digital I/O terminals. For more complex devices, parallel (FSMC, faster one) and SPI (in slave mode, noticeably slower) interfaces for slave device microcontroller (PDI) are available. It is offered in BGA 128 pin package. Along is offered its smaller brother ET1200, packed in QFN48, exposing SPI and half of GPIO. Both chips are at least 20$ per piece, and need external PHYs to work with Ethernet. For small I/O terminals common cost-saving setup is one terminal with Ethernet PHYs, RJ 45 connectors and following smaller terminals connected over local EBUS, but servodrives usually are connected with Ethernet cable and so PHYs are needed.
- More affordable option is LAN9252 from Microchip. Offered in 64 pin QFN and TQFP, it has 16 bit GPIO, or parallel interface (8 or 16 bit), or serial PDI: SPI or SQI, with much higher speed (at the cost of more complex register access). It has Ethernet PHY built in for cost saving and easier board design, and is sold for half the price of ET1100: 10-12$ (or was, before global chip shortage started)
- Then there are XMC4300 and XMC4800 from XMC: Cortex M4 microcontrollers with ESC builtin on AHB internal bus, PDI faster than FSMC. Fast option for higher end new design, not fit for this project (which for now aims to connect existing motor controllers, not create new one)
- Recently, AX58100 from ASIX arrived on the marked. It is licensed ET1100 derivative with improvements. It supports additional IO options: SPI master, Step-Dir controller, 3 phase PWM generator, encoder interface. Its SPI PDI supports higher clock speeds, IO lines are usually 5V tolerant, and chip includes builtin Ethernet PHYs. This chip is not available through western retailers.
- Finally, ASIX revealed AX58200: Cortex M4 based probably on Nuvoton M481W, with builtin ESC (and Ethernet PHYs). Chip is not offered by western retailers. On paper it looks like best choice for greenfield designs, but again this project aims not to be greenfield design.

We will start with cheap, simple to use ESC from known well brand that is available from typical vendors. That is Microchip LAN9252, and we will see how it fares

### Adapter board

First thing to consider is which ESC interface is going to be used. CiA 402 will not work with just GPIO, it requires some PDI. We are limited by exposed extension ports on existing OSS-OSHW motion control projects. STMBL exposes SPI1 of STM32F405. ODrive has SPI2 from same MCU. Parallel PDI, called HBI (high bandwidth interface) in LAN9252 docs and FSMC in STM32 docs, is faster, but would eat too many already used pins. We have to make it with SPI.

There are a few LAN 9252 dev boards with SPI exposed. There is range of official evaluation boards from Microchip. Then there is EasyCAT: third party Arduino shield with LAN9252, made by some Italian company that offers it with simple to use code generation tool (which unfortunately is not realy CoE compatible). Same company offers also EasyCAT Pro: small, bare minimum LAN9252 board with SPI breakout, even smaller than EVB-LAN9252-SPI. Its form factor is easier to embed into projects than Arduino shield. At 50 EUR it is pretty affordable, and would be good place to start.

Entire point of this project is to make high performance, end to end open source motion control. It will start from custom LAN9252-SPI board design 

## Software

### Select software stack

Only available open source solution is SOES, released under GPL ver. 2

### Execute TODO list

Stuff I already know will be needed:

- Make SPI adapter reach OP with blinky application
- CiA 402 device profile implementation
- uint test CiA402 state machine
- CiA 402 dummy project: loopback device (to test setups and serve as template)
- PR for OSS servo drive projects, adding EtherCAT support 
- documentation on setup with LinuxCNC

# Disclaimer

The EtherCAT Technology, the trade name and logo "EtherCAT" are the intellectual property of, and protected by Beckhoff Automation GmbH.

[Next: LAN9252-SPI PCB design](https://kubabuda.github.io/ecat_servo/002-lan9252-board-design)

[Back to the table of contents](https://kubabuda.github.io/ecat_servo)