# ------------------------------------------------------------------------------
# Set this config to the /dev path that your OS assigns to your target device
# ------------------------------------------------------------------------------
set(FLASH_DEV /dev/sdb1)

# ------------------------------------------------------------------------------
# Set this config to the path you would like to mount FLASH_DEV to
# ------------------------------------------------------------------------------
set(FLASH_MOUNT /media/devboard)

# ------------------------------------------------------------------------------
# Set this config to the path on the target device to install bootloader.bin to
# ------------------------------------------------------------------------------
set(FLASH_PATH boot)

# ------------------------------------------------------------------------------
# Build configs
# ------------------------------------------------------------------------------
set(BUILD_TARGET_ARCH aarch64)
set(CMAKE_BUILD_TYPE Debug)
set(BUILD_SHARED_LIBS OFF)
set(BUILD_STATIC_LIBS ON)
set(ENABLE_BUILD_VMM ON)
set(ENABLE_BUILD_USERSPACE OFF)
set(ENABLE_BUILD_EFI OFF)
set(ENABLE_BUILD_TEST OFF)
set(ENABLE_ASAN OFF)
set(ENABLE_TIDY OFF)
set(ENABLE_FORMAT OFF)
set(ENABLE_CODECOV OFF)
set(ENABLE_COMPILER_WARNINGS OFF)

# ------------------------------------------------------------------------------
# Board-specific configs
# ------------------------------------------------------------------------------
# include(${CMAKE_CURRENT_LIST_DIR}/scripts/cmake/config/jetson_tx1.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/scripts/cmake/config/jetson_tx2.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/scripts/cmake/config/sail.cmake)

# ------------------------------------------------------------------------------
# Add this extension to bareflank
# ------------------------------------------------------------------------------
set(EXTENSION ${CMAKE_CURRENT_LIST_DIR})
