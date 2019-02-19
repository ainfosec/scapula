# MIT License
#
# Copyright (c) 2018 Bareflank
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")

cmake_minimum_required(VERSION 3.12)
project(scapula)

include(${SOURCE_CMAKE_DIR}/project.cmake)
init_project()

find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(PYTHONPATH ${SCAPULA_SOURCE_ROOT_DIR}:${VMM_PREFIX_PATH}/lib/python3/dist-packages)

# set(SCAPULA_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/scapula)
set(SCAPULA_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(SCAPULA_STAMP_DIR ${SCAPULA_BUILD_DIR}/stamp)
file(MAKE_DIRECTORY ${SCAPULA_STAMP_DIR})

# ------------------------------------------------------------------------------
# Generate Scaupla Tests
# ------------------------------------------------------------------------------

file(GLOB_RECURSE SCAPULA_SRC_FILES "*.py")

set(SCAPULA_OUTPUT_STAMP ${SCAPULA_STAMP_DIR}/scapula_output.stamp)

add_custom_command(
    COMMAND PYTHONPATH=${PYTHONPATH} ${Python3_EXECUTABLE} -m scapula
    COMMAND cmake -E touch ${SCAPULA_OUTPUT_STAMP}
    OUTPUT ${SCAPULA_OUTPUT_STAMP}
    DEPENDS ${SCAPULA_SRC_FILES}
    COMMENT "Generating scapula outputs"
    VERBATIM
)

add_custom_target(generate ALL DEPENDS ${SCAPULA_OUTPUT_STAMP})

# ------------------------------------------------------------------------------
# Install
# ------------------------------------------------------------------------------

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR} DESTINATION lib/python3/dist-packages)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/ DESTINATION include/scapula)
