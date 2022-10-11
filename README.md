blink1-lib
==========
[![Build](https://github.com/evan1026/blink1-lib/actions/workflows/build.yml/badge.svg)](https://github.com/evan1026/blink1-lib/actions/workflows/build.yml)

This project is a C++ wrapper that uses classes and RAII principles to simplify
development of C++ code that interfaces with the [blink(1)](https://blink1.thingm.com/)
as compared to [the official blink1-tool C library](https://github.com/todbot/blink1-tool).

## Usage
Currently, this library supports being a CMake subdirectory, as well as building against shared or static
library files which can be found in the releases tab. I may also create a header-only version in the future for
those who prefer that.

### Shared or Static Library
Each release contains tar files with headers and library files which can be linked to your project.

### CMake Subdirectory
There are two main targets created:
* `blink1` - The main library
* `blink1-testing` - The testing library (see below)

Example usage:
```cmake
add_definitions(-DUSE_HIDAPI)  # blink1-tool requires either USE_HIDAPI or USE_HIDDATA to be set
                               # USE_HIDAPI is the default - see todbot/blink1-tool for more info
add_subdirectory(path/to/blink1-lib)

target_link_libraries(${EXECUTABLE_NAME} blink1)
target_link_libraries(${TEST_EXECUTABLE_NAME} blink1-testing)
```

By default, when this library is built as a subdirectory, it will build static libraries.
This behavior can be changed by setting the `BLINK1_LIB_BUILD_SHARED` option.

### CPM
Including with CPM is much the same as adding the library as a subdirectory, but CPM manages downloading it:
```cmake
add_definitions(-DUSE_HIDAPI)

CPMAddPackage(
  NAME blink1-lib
  GITHUB_REPOSITORY evan1026/blink1-lib
  VERSION 1.1.0
)

target_link_libraries(${EXECUTABLE_NAME} blink1)
target_link_libraries(${TEST_EXECUTABLE_NAME} blink1-testing)
```

To use the latest development version:
```cmake
CPMAddPackage(
  NAME blink1-lib
  GITHUB_REPOSITORY evan1026/blink1-lib
  VERSION latest
  GIT_TAG latest
)
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
