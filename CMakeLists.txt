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

include(${CMAKE_CURRENT_LIST_DIR}/scripts/cmake/config/default.cmake)
include(${SCAPULA_SOURCE_CMAKE_DIR}/validate.cmake)
include(${SCAPULA_SOURCE_CMAKE_DIR}/targets.cmake)

include_dependency(SCAPULA_SOURCE_DEPENDS_DIR dtc)
include_dependency(SCAPULA_SOURCE_DEPENDS_DIR libfdt)
include_dependency(SCAPULA_SOURCE_DEPENDS_DIR meson)
include_dependency(SCAPULA_SOURCE_DEPENDS_DIR libc)

add_subproject(
    shoulder vmm
    SOURCE_DIR ${SHOULDER_SOURCE_DIR}
)

add_subproject(
    scapula vmm
    DEPENDS shoulder
    SOURCE_DIR ${SCAPULA_SOURCE_ROOT_DIR}/scapula
)

add_subproject(
    bootloader vmm
    DEPENDS bfvmm
    DEPENDS bfintrinsics
    DEPENDS dtc
    DEPENDS libfdt
    DEPENDS shoulder
    DEPENDS scapula
    DEPENDS meson
    DEPENDS libc
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/bootloader/src
    TOOLCHAIN ${SOURCE_TOOLCHAIN_DIR}/gcc_aarch64_vmm.cmake
)
