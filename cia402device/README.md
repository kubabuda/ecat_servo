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
