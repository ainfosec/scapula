cmake_minimum_required(VERSION 3.6)
project(images C CXX)

include(${SOURCE_CMAKE_DIR}/project.cmake)
init_project()

# Device tree compiler (installed in the VMM prefix as a dependency)
unset(DTC_BIN CACHE)
find_program(DTC_BIN dtc HINTS ${CMAKE_INSTALL_PREFIX}/bin/)
if(DTC_BIN STREQUAL DTC_BIN-NOTFOUND)
    message(FATAL_ERROR "Device Tree Compiler not found!")
else()
    message(STATUS "Using Device Tree Compiler: ${DTC_BIN}")
endif()

# aarch64 cross-compiler objcopy
find_program(OBJCOPY_BIN aarch64-linux-gnu-objcopy HINTS /usr/bin/ /usr/local/bin)
if(OBJCOPY_BIN STREQUAL OBJCOPY_BIN-NOTFOUND)
    message(FATAL_ERROR "objcopy not found!")
else()
    message(STATUS "Using objcopy: ${OBJCOPY_BIN}")
endif()

# ------------------------------------------------------------------------------
# Platform flattened device tree / device tree binary (.dtb)
# The device tree that describes the target platform
# ------------------------------------------------------------------------------

if(EXISTS ${DEVICE_TREE_SOURCE})
    set(PLATFORM_DTS ${DEVICE_TREE_SOURCE})
elseif(EXISTS ${SCAPULA_DEVICE_TREE_DIR}/${DEVICE_TREE_SOURCE})
    set(PLATFORM_DTS ${SCAPULA_DEVICE_TREE_DIR}/${DEVICE_TREE_SOURCE})
else()
    message(FATAL_ERROR "Platform device tree source not found: ${DEVICE_TREE_SOURCE}")
endif()

set(PLATFORM_DTB ${CMAKE_CURRENT_BINARY_DIR}/device_tree.dtb)

add_custom_command(
    COMMAND ${DTC_BIN} --quiet --align 1024 -I dts -O dtb
    -o ${PLATFORM_DTB} ${PLATFORM_DTS}
    OUTPUT ${PLATFORM_DTB}
    DEPENDS ${PLATFORM_DTS}
    COMMENT "Creating device tree binary using: ${PLATFORM_DTS}"
)

add_custom_target(platform_dtb_target ALL DEPENDS ${PLATFORM_DTB})

install(FILES ${PLATFORM_DTB} DESTINATION boot)

# ------------------------------------------------------------------------------
# Testcase flattened image tree (.fit)
# Contains all sub-images to be used as the payload for the scapula bootloader
# ------------------------------------------------------------------------------

set(TESTCASE_ITS ${SCAPULA_SOURCE_ROOT_DIR}/scripts/device_tree/testcases.its)
set(TESTCASE_ITB ${CMAKE_CURRENT_BINARY_DIR}/scapula_payload.itb)

add_custom_command(
    COMMAND ${DTC_BIN} --quiet --align 1024 -I dts -O dtb
    -i ${CMAKE_INSTALL_PREFIX}/lib -o ${TESTCASE_ITB} ${TESTCASE_ITS}
    OUTPUT ${TESTCASE_ITB}
    DEPENDS ${TESTCASE_ITS}
    COMMENT "Creating testcase .fit image using: ${TESTCASE_ITS}"
)

add_custom_target(testcase_itb_target ALL DEPENDS ${TESTCASE_ITB})

install(FILES ${TESTCASE_ITB} DESTINATION boot)

# ------------------------------------------------------------------------------
# Bootloader raw binary (.bin)
# The main executable to be loaded on the target platform
# ------------------------------------------------------------------------------

set(BOOTLOADER_BIN ${CMAKE_CURRENT_BINARY_DIR}/bootloader.bin)
set(BOOTLOADER_ELF ${CMAKE_INSTALL_PREFIX}/bin/bootloader)

add_custom_command(
    COMMAND ${OBJCOPY_BIN} -v -O binary
        --set-section-flags .bss=alloc,load,contents
        ${BOOTLOADER_ELF} ${BOOTLOADER_BIN}
    OUTPUT ${BOOTLOADER_BIN}
    DEPENDS ${BOOTLOADER_ELF}
    COMMENT "Creating bootloader binary: ${BOOTLOADER_BIN}"
)

add_custom_target(bootloader_bin ALL DEPENDS ${BOOTLOADER_BIN})

install(FILES ${BOOTLOADER_BIN} DESTINATION boot)
