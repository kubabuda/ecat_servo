# CiA 402 device

This project is tested open source implementation of CANOpen CiA402 profile (motion control).
Axis struct has states, transitions and action flags, its up for application to act on these.

```c
cia402_axis_t cia402axis;                           // create instance of motion control axis
cia402_initialize(&cia402axis, &OD.Status_Word, &mockOD.ALstatus);  // initalize
// ...
while (true) {                                      // in main loop
    // ...                                          // read PDOs from CANbus, EtherCAT...
    cia402_state_machine(&cia402axis, controlword); // process SM

    if (axis.flags.axis_func_enabled) { /* implement your drive following command */ }
    if (axis->flags.hv_power_applied) { /* enable HV power if applicable */ }
    if (axis->flags.brake_applied) { /* release brake if applicable */ }

    if(/* application detected error condition */) {
        cia402axis.state = FAULT_REACTION_ACTIVE;  // trigger fault reaction
        cia402_state_machine(&cia402axis, controlword); // process SM
    }
}
```

Unit tests can be ran natively on PC (for this one needs to [install GCC and add it to PATH](https://piolabs.com/blog/insights/unit-testing-part-2.html))


### TODO 

- structure unit tests like this https://ucgosu.pl/2018/04/unity-framework-testowy-w-c/
- test coverage https://piolabs.com/blog/insights/test-coverage-on-unit-testing.html https://github.com/Strooom/demoTestCoverage https://embetronicx.com/tutorials/unit_testing/unit-testing-in-c-testing-with-unity/
- run tests on target HW: STM32F40x https://piolabs.com/blog/insights/unit-testing-part-2.html


How to build PlatformIO based project
=====================================

1. [Install PlatformIO Core](https://docs.platformio.org/page/core.html)
2. Download [development platform with examples](https://github.com/platformio/platform-native/archive/develop.zip)
3. Extract ZIP archive
4. Run these commands:

```shell
# Change directory to example
$ cd platform-native/examples/hello-world

# Build project
$ pio run

# Execute program
$ pio run --target exec
# or using embedded's "upload" target
$ pio run --target upload

# Clean build files
$ pio run --target clean
```



