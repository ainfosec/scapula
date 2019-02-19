# Scapula

Searching Concrete ARM Processors for Underlying Lousy Architecture

This repository functions as an aarch64 bootloader extension for the
[Bareflank Hypervisor SDK](https://github.com/Bareflank/hypervisor).
Mainline Bareflank does not offically support any versions of the ARM
architecture yet, but it is far enough along for the purposes of this project

Hardware support initially targets the NVIDIA Jetson TX1 and TX2 development
kit. Eventually, the hope is to add support for some more readily available
environments like Raspberry Pi 3 and QEMU.

## Dependencies

This extension requires CMake version > 3.12, and in addition to Bareflank's
usual dependencies:

Ubuntu:

```
sudo apt-get install gcc-aarch64-linux-gnu python3
```

## Building

The quick way to build: (replace '\<full_path_to_this_repo>' with a real path) 
```
git clone --recursive https://gitlab.ainfosec.com/bareflank/tlp2_projects/scapula
git clone https://github.com/bareflank/shoulder
mkdir build && cd build
cmake ../scapula/hypervisor -DCONFIG=/<full_path_to_this_repo>/config.cmake
make
```
