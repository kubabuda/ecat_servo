# CiA 402 device

This project is open source implementation of CANOpen CiA402 profile (motion control), with unit tests.

### TODO 

- structure unit tests like this https://ucgosu.pl/2018/04/unity-framework-testowy-w-c/
- remove hooks - axis struct has states and transitions, its up for application to act on them
- maybe init should be dropped too
- refactor command enum to contain command value
- replace is_command with macro


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
