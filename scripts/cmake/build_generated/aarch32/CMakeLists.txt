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
project(aarch32_testcases C)
include(${SCAPULA_SOURCE_CMAKE_DIR}/init_subproject.cmake)

generate_flags(aarch32)

# ------------------------------------------------------------------------------
# 32-bit test case library
# ------------------------------------------------------------------------------

file(GLOB_RECURSE AARCH32_TESTCASE_SRC_FILES ${CMAKE_INSTALL_PREFIX}/src/*/aarch32/*.c)

add_library(aarch32_testcases ${AARCH32_TESTCASE_SRC_FILES})

install(TARGETS aarch32_testcases DESTINATION lib)
