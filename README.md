# Scapula

This repository contains tools for performing testing and verification of ARM
CPUs against a
[machine parsable version of the ARMv8-A Architecture Reference Manual](https://developer.arm.com/architectures/cpu-architecture/a-profile/exploration-tools).

The project consists of three major components:

1. Scapula: A python package that makes it easy to generate executable test
cases using the information contained in the manuals

2. Scapula OS: A minimal runtime enviornment for test cases generated by Scapula

3. Scapula Loader: A bootloader that packages up and loads the other components onto an ARM CPU

Generated test cases run as a single thread, bare-metal application that
spans all exception levels (EL0, EL1, EL2, EL3). This allows tests to
verify behaviors of a CPU that cross typical software isolation boundaries:

```
void example_test(void)
{
    switch_to_el(2);
    printf("This test is executing as a hypervisor in EL2\n");

    switch_to_el(1);
    printf("Now the test is executing as an OS in EL1\n");

    aarch64_sctlr_el3_get();
    printf("The test is now in EL%u, ", aarch64_currentel_el_get()); 
    printf("because the above register access caused a synchronous exception\n");
}
```

Support for crossing execution states (aarch32 and aarch64) and security states 
(secure world, non-secure world) are currently in the works.

## Dependencies

Scapula officially supports building on Ubuntu 19.04+ using the following
dependencies:

```
sudo apt-get install cmake gcc-aarch64-linux-gnu arm-linux-gnueabihf-gcc python3 python3-pip
sudo pip3 install lxml colorama 
```

CMake version > 3.12 is required.

## Building

The easiest way to build:

```
git clone https://github.com/ainfosec/scapula.git
mkdir build && cd build
cmake ../scapula
make
```

The better way to build:

```
mkdir workspace && cd workspace
git clone https://github.com/ainfosec/scapula.git
mkdir cache             # <-- Dependencies will download here, shared across builds
touch config.cmake      # <-- Edit this file to define your build configurations
mkdir build && cd build
cmake ../scapula
make -j<# of cores on your computer>
```

## Configuring

Scapula uses CMake to configure build options. There are numerous ways that you
can specify build options to CMake:
  
1. Display all available options and configure the project with a menu based
assistant:

```
sudo apt-get install cmake-curses-gui
ccmake </path/to/a/configured/build/directory>
```

2. Configure options using the command line:

```
cmake ../scapula -DSYSREG_XML_PATH=~/workspace/sysregs
```

3. Configure options using a CMake config file. For example, create a file with
the following contents and save it to ~/workspace/config.cmake:

```
set(SYSREG_XML_PATH ~/workspace/sysregs)
```

then configure the build using:

```
cmake ../scapula -DCONFIG=~/workspace/config.cmake
```

In general, it is best to use only one configuration mechanism per build. If you
do not specify any build configurations, the project will use the default
options defined in ``` scapula/scripts/cmake/config/default.cmake ```.

## Supported Platforms

Offical hardware support currently includes the NVIDIA Jetson TX1 and TX2
development kits, as well as an
[emulator derived from a Sail model](https://github.com/rems-project/sail-arm/tree/master/arm-v8.5-a).

However, porting to a new hardware platform could be as simple as modifying a
few build configuraiton options (assuming memory is already initialized):

```
# The base address of a serial port to be used for printing results
set(SERIAL_BASE 0x3c000000)

# The type of serial port, options: [8250_uart, pl011_primecell]
set(SERIAL_DEVICE pl011_primecell)

# The address that Scapula Loader will be loaded to by a preveous stage bootloader
set(START_ADDRESS 0x80000000)

# The amount of memory Scapula OS will use as a heap
set(HEAP_SIZE 0x10000)

# The amount of memory Scapula OS will use as a stack
set(STACK_SIZE 0x10000)
```
