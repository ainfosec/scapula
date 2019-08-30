cmake_minimum_required(VERSION 3.6)
project(images C CXX)
include(${SCAPULA_SOURCE_CMAKE_DIR}/init_subproject.cmake)

# Device tree compiler
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
# Contains all sub-images to be used as the payload for Scapula OS 
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
# Scapula OS raw binary (.bin)
# The main executable to be loaded on the target platform
# ------------------------------------------------------------------------------

set(SCAPULA_OS_BIN ${CMAKE_CURRENT_BINARY_DIR}/scapula_os.bin)
set(SCAPULA_OS_ELF ${CMAKE_INSTALL_PREFIX}/bin/scapula_os)

add_custom_command(
    COMMAND ${OBJCOPY_BIN} -v -O binary
        --set-section-flags .bss=alloc,load,contents
        ${SCAPULA_OS_ELF} ${SCAPULA_OS_BIN}
        OUTPUT ${SCAPULA_OS_BIN}
        DEPENDS ${SCAPULA_OS_ELF}
        COMMENT "Creating scapula_os binary: ${SCAPULA_OS_BIN}"
)

add_custom_target(scapula_os_bin ALL DEPENDS ${SCAPULA_OS_BIN})

install(FILES ${SCAPULA_OS_BIN} DESTINATION boot)
