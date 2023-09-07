# ECAT servo
## Free EtherCAT CiA402 servo drive implementation on STM32 (F1, F4), with Arduino example

EtherCAT is realtime Ethernet protocol for automation. It lets you control standard industrial equipment with your PC or laptop, without interface cards or special adapters. All it needs is just standard Ethernet card with RJ45 port. It offers low, very predictable latency, so it perfect motion control network for building modern CNC, robots and other machines. The most popular EtherCAT application protocol for servo drives seems to be CANopen (over EtherCAT) CiA402. 

This project is free implementation of CoE CiA402 standard, for use with open motor controllers like ODrive, STMBL, or your next thing. Along the way, it shows how to do EtherCAT device development using free and open source tools.

# [Sources](https://github.com/kubabuda/ecat_servo/tree/main/examples/SOES_CIA402_AX58100)

CiA 402 implementation [is here, complete with test coverage](https://github.com/kubabuda/ecat_servo/tree/main/cia402device)

Example application for STM32 is [is here](https://github.com/kubabuda/ecat_servo/tree/main/examples/SOES_CIA402_AX58100)

[./examples](https://github.com/kubabuda/ecat_servo/tree/main/examples) dir contains platform.io projects showing how to use LAN9252 and AX58100 ESCs with STM32 and [SOES library](https://github.com/OpenEtherCATsociety/SOES).


# [Documentation](https://kubabuda.github.io/ecat_servo/001-intro)

[Documentation](https://kubabuda.github.io/ecat_servo/001-intro) in /docs contains progress log with rationale on some decisions

# [Hackaday](https://hackaday.io/project/181058-ecatservo)

[Hackaday.io project is here](https://hackaday.io/project/181058-ecatservo) for more convinient discussion 

# Disclaimer

The EtherCAT Technology, the trade name and logo "EtherCAT" are the intellectual property of, and protected by Beckhoff Automation GmbH.
