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

set(AARCH32_TARGET_TRIPLE arm-none-elf
    CACHE INTERNAL
    "Target triple for aarch32 bare-metal binaries"
)

set(SCAPULA_AARCH32_INSTALL_PREFIX ${PREFIXES_DIR}/${AARCH32_TARGET_TRIPLE}
    CACHE INTERNAL
    "Install prefix for aarch32 bare-metal binaries"
)

set(AARCH64_TARGET_TRIPLE aarch64-none-elf
    CACHE INTERNAL
    "Target triple for aarch64 bare-metal binaries"
)

set(SCAPULA_AARCH64_INSTALL_PREFIX ${PREFIXES_DIR}/${AARCH64_TARGET_TRIPLE}
    CACHE INTERNAL
    "Install prefix for aarch64 bare-metal binaries"
)

set(HOST_TARGET_TRIPLE ${CMAKE_HOST_SYSTEM_PROCESSOR}-${CMAKE_HOST_SYSTEM_NAME}-elf
    CACHE INTERNAL
    "Target triple for host system binaries"
)

set(SCAPULA_HOST_INSTALL_PREFIX ${PREFIXES_DIR}/${HOST_TARGET_TRIPLE}
    CACHE INTERNAL
    "Install prefix for host system binaries"
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
    DESCRIPTION "Path to flash scapula_os.bin on target device ${FLASH_DEV}"
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
    CONFIG_NAME START_ADDRESS
    CONFIG_TYPE STRING
    DEFAULT_VAL 0x80000000
    DESCRIPTION "Address that Scapula OS starts execution at"
)

add_config(
    CONFIG_NAME HEAP_SIZE
    CONFIG_TYPE STRING
    DEFAULT_VAL 0x200000
    DESCRIPTION "Size (in bytes) that Scapula OS will use for heap memory"
)

add_config(
    CONFIG_NAME STACK_SIZE
    CONFIG_TYPE STRING
    DEFAULT_VAL 0x10000
    DESCRIPTION "Size (in bytes) that Scapula OS will use for stack memory"
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

add_config(
    CONFIG_NAME LOG_LEVEL
    CONFIG_TYPE STRING
    DEFAULT_VAL 3
    DESCRIPTION "Log-level (0=none, 1=error, 2=warn, 3=info, 4=debug)"
    OPTIONS 0 1 2 3 4
)
