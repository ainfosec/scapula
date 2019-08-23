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

if(CMAKE_INSTALL_PREFIX)
    set(ENV{CMAKE_INSTALL_PREFIX} "${CMAKE_INSTALL_PREFIX}")
else()
    set(CMAKE_INSTALL_PREFIX "$ENV{CMAKE_INSTALL_PREFIX}")
endif()

set(CMAKE_SYSTEM_NAME Linux)
set(CROSS_COMPILE_TRIPLE arm-linux-gnueabihf)

string(CONCAT LD_FLAGS
    "-L${CMAKE_INSTALL_PREFIX}/lib "
    "-nostdlib "
)

find_program(CMAKE_C_COMPILER ${CROSS_COMPILE_TRIPLE}-gcc)
if(NOT CMAKE_C_COMPILER)
    message(FATAL_ERROR "Unable to find ${CROSS_COMPILE_TRIPLE}-gcc")
else()
    set(CMAKE_CXX_COMPILER ${CMAKE_C_COMPILER})
endif()

find_program(AS_BIN ${CROSS_COMPILE_TRIPLE}-as)
if(NOT AS_BIN)
    message(FATAL_ERROR "Unable to find ${CROSS_COMPILE_TRIPLE}-as")
endif()

find_program(LD_BIN ${CROSS_COMPILE_TRIPLE}-ld)
if(NOT LD_BIN)
    message(FATAL_ERROR "Unable to find ${CROSS_COMPILE_TRIPLE}-ld")
endif()

find_program(LD_BIN ${CROSS_COMPILE_TRIPLE}-ld)
if(NOT LD_BIN)
    message(FATAL_ERROR "Unable to find ${CROSS_COMPILE_TRIPLE}-ld")
endif()

find_program(AR_BIN ${CROSS_COMPILE_TRIPLE}-ar)
if(NOT AR_BIN)
    message(FATAL_ERROR "Unable to find ${CROSS_COMPILE_TRIPLE}-ar")
endif()

set(CMAKE_C_ARCHIVE_CREATE
    "${AR_BIN} qc <TARGET> <OBJECTS>"
)

set(CMAKE_CXX_ARCHIVE_CREATE
    "${AR_BIN} qc <TARGET> <OBJECTS>"
)

set(CMAKE_C_LINK_EXECUTABLE
    "${LD_BIN} ${LD_FLAGS} -pie <OBJECTS> -o <TARGET> <LINK_LIBRARIES> "
)

set(CMAKE_CXX_LINK_EXECUTABLE
    "${LD_BIN} ${LD_FLAGS} -pie <OBJECTS> -o <TARGET> <LINK_LIBRARIES>"
)

set(CMAKE_C_CREATE_SHARED_LIBRARY
    "${LD_BIN} ${LD_FLAGS} -shared <OBJECTS> -o <TARGET>"
)

set(CMAKE_CXX_CREATE_SHARED_LIBRARY
    "${LD_BIN} ${LD_FLAGS} -shared <OBJECTS> -o <TARGET>"
)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
