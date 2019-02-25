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

cmake_minimum_required(VERSION 3.6)
project(bootloader C CXX)

# ------------------------------------------------------------------------------
# Extension setup and configuration
# ------------------------------------------------------------------------------

include(${SOURCE_CMAKE_DIR}/project.cmake)
init_project(
    INCLUDES ${PROJECT_SOURCE_DIR}/../include
    INCLUDES ${PROJECT_SOURCE_DIR}/../../bfsdk/include
)

# TODO: Eventually, we need to support all of these flags, they are removed
# while devloping an initial proof-of-concept
string(REPLACE "-std=c11 " "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
string(REPLACE "-Wall " "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
string(REPLACE "-Werror " "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
string(REPLACE "-Wconversion " "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
string(REPLACE "-Wpedantic " "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
string(REPLACE "-Wextra " "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
string(REPLACE "-Wsign-conversion " "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
string(REPLACE "--target=${BUILD_TARGET_ARCH}-vmm-elf " "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
string(REPLACE "--target=${BUILD_TARGET_ARCH}-vmm-elf " "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mgeneral-regs-only")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-stack-protector")

# ------------------------------------------------------------------------------
# Main bootloader elf executable
# ------------------------------------------------------------------------------

list(APPEND BOOTLOADER_SRC_FILES
    start.s
    main.c
    bootloader.c
    microlib.c
    printf.c
    uart_tegra.s
    interrupt_vectors.s
    interrupt_vectors.c
    switch_exception_level.s
    switch_exception_level.c
)

# add_vmm_executable(bootloader SOURCES ${BOOTLOADER_SRC_FILES})
add_executable(bootloader_static ${BOOTLOADER_SRC_FILES})
set(BOOTLOADER_LINKER_SCRIPT "${SCAPULA_SOURCE_ROOT_DIR}/scripts/linker/bootloader.lds")
set_target_properties(bootloader_static PROPERTIES LINK_DEPENDS ${BOOTLOADER_LINKER_SCRIPT})
target_link_libraries(bootloader_static -T ${BOOTLOADER_LINKER_SCRIPT})
# set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_LINK_EXECUTABLE} -T ${BOOTLOADER_LINKER_SCRIPT}")
set(BOOTLOADER_ELF ${CMAKE_CURRENT_BINARY_DIR}/bootloader_static)

# ------------------------------------------------------------------------------
# Platform flattened device tree / device tree binary (.dtb)
# ------------------------------------------------------------------------------

# Device tree compiler (installed in the VMM prefix as a dependency)
unset(DTC_BIN CACHE)
find_program(DTC_BIN dtc HINTS ${CMAKE_INSTALL_PREFIX}/bin/)
if(DTC_BIN STREQUAL DTC_BIN-NOTFOUND)
    message(FATAL_ERROR "Device Tree Compiiler not found!")
else()
    message(STATUS "Using Device Tree Compiler: ${DTC_BIN}")
endif()

set(DEVICE_TREE_BINARY ${CMAKE_CURRENT_BINARY_DIR}/device_tree.dtb)
add_custom_command(
    COMMAND ${DTC_BIN} --quiet --align 1024 -I dts -O dtb
        -o ${DEVICE_TREE_BINARY} ${DEVICE_TREE_SOURCE}
    OUTPUT ${DEVICE_TREE_BINARY}
    DEPENDS ${DEVICE_TREE_SOURCE}
    COMMENT "Creating device tree binary using: ${DEVICE_TREE_SOURCE}"
)
add_custom_target(bootloader_dtb ALL DEPENDS ${DEVICE_TREE_BINARY})
install(FILES ${DEVICE_TREE_BINARY} DESTINATION boot)

# ------------------------------------------------------------------------------
# Bootloader raw binary (.bin)
# ------------------------------------------------------------------------------

set(BOOTLOADER_BIN ${CMAKE_CURRENT_BINARY_DIR}/bootloader.bin)
add_custom_command(
    COMMAND ${CMAKE_OBJCOPY} -v -O binary
        --set-section-flags .bss=alloc,load,contents
        ${BOOTLOADER_ELF} ${BOOTLOADER_BIN}
    OUTPUT ${BOOTLOADER_BIN}
    DEPENDS ${BOOTLOADER_ELF}
    COMMENT "Creating bootloader binary: ${BOOTLOADER_BIN}"
)
add_custom_target(bootloader_bin ALL DEPENDS ${BOOTLOADER_BIN})
if(BUILD_IMAGE_FORMAT STREQUAL "bin")
    install(FILES ${BOOTLOADER_BIN} DESTINATION boot)
endif()

# ------------------------------------------------------------------------------
# Bootloader shellcode
# ------------------------------------------------------------------------------

if(BUILD_IMAGE_FORMAT STREQUAL "shellcode")
    # TODO
endif()

# ------------------------------------------------------------------------------
# Bootloader flattened image tree (.fit)
# ------------------------------------------------------------------------------

if(BUILD_IMAGE_FORMAT STREQUAL "fit")
    # TODO: Package up bootloader, device tree, and a kernel into bootloader.fit
endif()
