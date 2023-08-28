# CiA 402 device

This project is open source implementation of CANOpen CiA402 profile (motion control), with unit tests.
Axis struct has states and transitions, its up for application to act on these

### TODO 

- unit test status word changes
- structure unit tests like this https://ucgosu.pl/2018/04/unity-framework-testowy-w-c/
- test coverage https://piolabs.com/blog/insights/test-coverage-on-unit-testing.html https://github.com/Strooom/demoTestCoverage https://embetronicx.com/tutorials/unit_testing/unit-testing-in-c-testing-with-unity/
- run on target HW: STM32F40x https://piolabs.com/blog/insights/unit-testing-part-2.html


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
