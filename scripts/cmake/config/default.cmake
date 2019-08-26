#
# Bareflank Hypervisor
# Copyright (C) 2018 Assured Information Security, Inc.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

# ------------------------------------------------------------------------------
# Constants
# ------------------------------------------------------------------------------

set(SCAPULA_SOURCE_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../../..
    CACHE INTERNAL
    "Scapula source root directory"
)

set(SCAPULA_SOURCE_CMAKE_DIR ${SCAPULA_SOURCE_ROOT_DIR}/scripts/cmake
    CACHE INTERNAL
    "Scapula cmake scripts directory"
)

set(SCAPULA_DEVICE_TREE_DIR ${SCAPULA_SOURCE_ROOT_DIR}/scripts/device_tree
    CACHE INTERNAL
    "Scapula device tree directory"
)

set(SCAPULA_SOURCE_CONFIG_DIR ${CMAKE_CURRENT_LIST_DIR}
    CACHE INTERNAL
    "Scapula cmake configuration directory"
)

set(SCAPULA_SOURCE_DEPENDS_DIR ${SCAPULA_SOURCE_CMAKE_DIR}/depends
    CACHE INTERNAL
    "Scapula cmake dependencies directory"
)

set(PREFIXES_DIR ${CMAKE_BINARY_DIR}/prefixes
    CACHE INTERNAL
    "Prefixes directory"
)

set(AARCH64_TARGET_TRIPLE aarch64-none-elf
    CACHE INTERNAL
    "Target triple for aarch64 bare-metal binaries"
)

set(SCAPULA_AARCH64_INSTALL_PREFIX ${PREFIXES_DIR}/${AARCH64_TARGET_TRIPLE}
    CACHE INTERNAL
    "Install prefix for aarch64 bare-metal binaries"
)

set(DEPENDS_DIR ${CMAKE_BINARY_DIR}/depends
    CACHE INTERNAL
    "External dependencies directory"
)

set(CMAKE_BUILD_TYPE "Release"
    CACHE INTERNAL
    "Defines the build type"
)

set(CMAKE_VERBOSE_MAKEFILE OFF
    CACHE INTERNAL
    "Enables verbose output"
)
# ------------------------------------------------------------------------------
# Configs
# ------------------------------------------------------------------------------

add_config(
    CONFIG_NAME BUILD_TARGET_ARCH
    CONFIG_TYPE STRING
    DEFAULT_VAL aarch64
    DESCRIPTION "The target architecture for the build"
    OPTIONS aarch64
)

add_config(
    CONFIG_NAME BUILD_IMAGE_FORMAT
    CONFIG_TYPE STRING
    DEFAULT_VAL bin
    DESCRIPTION "The target image format"
    OPTIONS bin fit
)

add_config(
    CONFIG_NAME DEVICE_TREE_SOURCE
    CONFIG_TYPE STRING
    DEFAULT_VAL ${SCAPULA_DEVICE_TREE_DIR}/sail.dts
    DESCRIPTION "The platform device tree source file"
)

add_config(
    CONFIG_NAME FLASH_DEV
    CONFIG_TYPE STRING
    DEFAULT_VAL /dev/sdb1
    DESCRIPTION "The device node to use for 'make flash' target"
)

add_config(
    CONFIG_NAME FLASH_MOUNT
    CONFIG_TYPE PATH
    DEFAULT_VAL /media/jetson
    DESCRIPTION "Path to mount FLASH_DEV to during 'make flash' target"
    SKIP_VALIDATION
)

add_config(
    CONFIG_NAME FLASH_PATH
    CONFIG_TYPE PATH
    DEFAULT_VAL boot
    DESCRIPTION "Path to flash bootloader.bin on target device ${FLASH_DEV}"
    SKIP_VALIDATION
)

add_config(
    CONFIG_NAME SERIAL_BASE
    CONFIG_TYPE STRING
    DEFAULT_VAL 0x3c000000
    DESCRIPTION "The serial port base address"
)

add_config(
    CONFIG_NAME SERIAL_DEVICE
    CONFIG_TYPE STRING
    DEFAULT_VAL pl011_primecell
    DESCRIPTION "The model of serial device used for printing messages"
    OPTIONS 8250_uart pl011_primecell
)

add_config(
    CONFIG_NAME BOOTLOADER_START_ADDRESS
    CONFIG_TYPE STRING
    DEFAULT_VAL 0x80000000
    DESCRIPTION "Address for the bootloader to start at"
)

set(DEFAULT_CACHE_DIR ${SCAPULA_SOURCE_ROOT_DIR}/../cache
    CACHE INTERNAL
    "Default cache directory"
)

if(EXISTS ${DEFAULT_CACHE_DIR})
    get_filename_component(DEFAULT_CACHE_DIR "${DEFAULT_CACHE_DIR}" ABSOLUTE)
else()
    set(DEFAULT_CACHE_DIR ${CMAKE_BINARY_DIR}/cache)
endif()

add_config(
    CONFIG_NAME CACHE_DIR
    CONFIG_TYPE PATH
    DEFAULT_VAL ${DEFAULT_CACHE_DIR}
    DESCRIPTION "Cache directory"
    SKIP_VALIDATION
)

add_config(
    CONFIG_NAME SYSREG_XML_PATH
    CONFIG_TYPE PATH
    DEFAULT_VAL NONE
    DESCRIPTION "Path to system register XML spec directory (or NONE)"
    SKIP_VALIDATION
)

add_config(
    CONFIG_NAME A32_XML_PATH
    CONFIG_TYPE PATH
    DEFAULT_VAL NONE
    DESCRIPTION "Path to A32 instruction XML spec directory (or NONE)"
    SKIP_VALIDATION
)

add_config(
    CONFIG_NAME A64_XML_PATH
    CONFIG_TYPE PATH
    DEFAULT_VAL NONE
    DESCRIPTION "Path to A64 instruction XML spec directory (or NONE)"
    SKIP_VALIDATION
)

# ------------------------------------------------------------------------------
# Links
# ------------------------------------------------------------------------------

set(DTC_URL "https://github.com/dgibson/dtc/archive/v1.4.6.zip"
    CACHE INTERNAL
    "Device tree compiler/libfdt URL"
)

set(DTC_URL_MD5 "540fb180485cd98b73800d39f2993a29"
    CACHE INTERNAL
    "Device tree compiler/libfdt URL MD5 hash"
)

set(SHOULDER_URL "https://github.com/bareflank/shoulder/archive/master.zip"
    CACHE INTERNAL
    "Shoulder URL"
)

set(SHOULDER_URL_MD5 "3e33d7fc7afaffd0a51645c8b149905a"
    CACHE INTERNAL
    "Shoulder URL MD5 hash"
)

set(LIBC_URL "https://github.com/embeddedartistry/libc.git"
    CACHE INTERNAL
    "Embedded Artistry Libc URL"
)

set(LIBMEMORY_URL "https://github.com/embeddedartistry/libmemory.git"
    CACHE INTERNAL
    "Embedded Artistry Libmemory URL"
)

set(MESON_URL "https://github.com/mesonbuild/meson/releases/download/0.51.1/meson-0.51.1.tar.gz"
    CACHE INTERNAL
    "Meson build system URL"
)

set(MESON_URL_MD5 "48787e391ec5c052799a3dd491f73909"
    CACHE INTERNAL
    "Meson build system URL MD5"
)

set(SYSREG_SPEC_URL
    "https://developer.arm.com/-/media/Files/ATG/Beta10/SysReg_xml_v85A-2019-06.tar.gz"
    CACHE INTERNAL
    "URL for downloading the system register XML specification"
)

set(SYSREG_SPEC_MD5 "c2ac0794b6b47e8a07d6866e770980eb"
    CACHE INTERNAL
    "Sysreg spec URL MD5"
)

set(A64_SPEC_URL
    "https://developer.arm.com/-/media/Files/ATG/Beta10/A64_ISA_xml_v85A-2019-06.tar.gz"
    CACHE INTERNAL
    "URL for downloading the A64 instruction set XML specification"
)

set(A64_SPEC_MD5 "109d10b644bf86760d65ded3193445e8"
    CACHE INTERNAL
    "A64 spec URL MD5"
)

set(A32_SPEC_URL
    "https://developer.arm.com/-/media/Files/ATG/Beta10/AArch32_ISA_xml_v85A-2019-06.tar.gz"
    CACHE INTERNAL
    "URL for downloading the A32 instruction set XML specification"
)

set(A32_SPEC_MD5 "139d1a73f6a414a7309f65bbd5752d67"
    CACHE INTERNAL
    "A32 spec URL MD5"
)
