blink1-lib
==========
[![build](https://github.com/evan1026/blink1-lib/actions/workflows/ccpp.yml/badge.svg?branch=master)](https://github.com/evan1026/blink1-lib/actions/workflows/ccpp.yml)

This project is a C++ wrapper that uses classes and RAII principles to simplify
development of C++ code that interfaces with the [blink(1)](https://blink1.thingm.com/)
as compared to [the official blink1-tool C library](https://github.com/todbot/blink1-tool).

## Usage
At the moment, the only way to use this library is to include it as a CMake
subdirectory. I have future plans to create releases with static and dynamic
library files, as well as possibly a header-only release.

### CMake Subdirectory
When included in a CMake project, this library will export a few variables:
* `BLINK1_LIB_INCLUDES` - include files for accessing library headers
* `BLINK1_LIB_LIBRARIES` - libraries that need to be added to any target that interfaces with an actual blink(1) device
* `BLINK1_LIB_TESTING_LIBRARIES` - libraries used for testing - see below

Example usage:
```cmake
...

add_definitions(-DUSE_HIDAPI)  # blink1-tool requires either USE_HIDAPI or USE_HIDDATA to be set
                               # USE_HIDAPI is the default - see todbot/blink1-tool for more info
add_subdirectory(path/to/blink1-lib)
include_directories(${BLINK1_LIB_INCLUDES})

target_link_libraries(${EXECUTABLE_NAME} ${BLINK1_LIB_LIBRARIES})
target_link_libraries(${TEST_EXECUTABLE_NAME} ${BLINK1_LIB_TESTING_LIBRARIES})
```

## Testing Library
In addition to providing a wrapper around the blink1-tool library, this project
also provides a testing library to simplify unit-testing and development for blink(1)
devices. This library is specifically designed to work with gtest, but I could update
it to be more generic if there's any desire for that. It can also be used for
development so that blink(1) code can be run without a device needing to be present,
or to test your program's behavior under special cases that are hard to recreate
with a real device.

In order to use this library, include `Blink1TestingLibrary.hpp` in your test case
and make sure you link against `${BLINK1_LIB_TESTING_LIBRARIES}`.

Further information about the library's features can be found [here](https://evan1026.github.io/blink1-lib/docs/namespacefake__blink1__lib.html).

## Docs
Class documentation can be found [here](https://evan1026.github.io/blink1-lib/docs/index.html)
