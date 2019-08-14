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
project(aarch64_testcases C)

# ------------------------------------------------------------------------------
# Extension setup and configuration
# ------------------------------------------------------------------------------

include(${SOURCE_CMAKE_DIR}/project.cmake)
init_project()

# Flags removed from Bareflank's defaults
string(REPLACE "-std=c11 " "-std=gnu11 " CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
string(REPLACE "--target=${BUILD_TARGET_ARCH}-vmm-elf " "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})

# Flags added
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mgeneral-regs-only")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-stack-protector")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv8-a")

# ------------------------------------------------------------------------------
# 64-bit test case library
# ------------------------------------------------------------------------------

file(GLOB_RECURSE AARCH64_TESTCASE_SRC_FILES ${VMM_PREFIX_PATH}/src/*/aarch64/*.c)

add_library(aarch64_testcases ${AARCH64_TESTCASE_SRC_FILES})

install(TARGETS aarch64_testcases DESTINATION lib)
