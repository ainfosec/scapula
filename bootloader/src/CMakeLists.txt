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

# ------------------------------------------------------------------------------
# Main bootloader elf executable
# ------------------------------------------------------------------------------

list(APPEND BOOTLOADER_SRC_FILES
    start.s
    main.c
    bootloader.c
    testcase_loader.c
    serial_8250_mini.c
    interrupt_vectors.s
    interrupt_vectors.c
    switch_exception_level.s
    switch_exception_level.c
)

if(SERIAL_DEVICE STREQUAL "8250_mini")
    list(APPEND BOOTLOADER_SRC_FILES
        serial_8250_mini.c
    )
elseif(SERIAL_DEVICE STREQUAL "pl011_primecell")
    list(APPEND BOOTLOADER_SRC_FILES
        serial_pl011_primecell.c
    )
else()
    message(FATAL_ERROR "Serial device not found: ${SERIAL_DEVICE}")
endif()

add_executable(bootloader ${BOOTLOADER_SRC_FILES})

target_compile_definitions(bootloader
    PRIVATE BOOTLOADER_CONFIG_SERIAL_BASE=${SERIAL_BASE}
)

set(BOOTLOADER_LINKER_SCRIPT "${SCAPULA_SOURCE_ROOT_DIR}/scripts/linker/bootloader.lds")
set_target_properties(bootloader PROPERTIES LINK_DEPENDS ${BOOTLOADER_LINKER_SCRIPT})
target_link_libraries(bootloader -T ${BOOTLOADER_LINKER_SCRIPT})

list(APPEND LIBRARIES
    ${CMAKE_INSTALL_PREFIX}/lib/libfdt.a
    ${CMAKE_INSTALL_PREFIX}/lib/libc.a
    ${CMAKE_INSTALL_PREFIX}/lib/libprintf.a
    ${CMAKE_INSTALL_PREFIX}/lib/libmemory_freelist.a
)

target_link_libraries(bootloader ${LIBRARIES})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/bootloader DESTINATION bin)
install(DIRECTORY ../include/ DESTINATION include/bootloader)
