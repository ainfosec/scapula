#
# Bareflank Hypervisor
# Copyright (C) 2015 Assured Information Security, Inc.
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

unset(AARCH64_FLAGS)
unset(AARCH64_C_FLAGS)
unset(AARCH64_CXX_FLAGS)

list(APPEND AARCH64_FLAGS
    -fpic
    -fno-stack-protector 
    -fno-builtin
    -fno-common
    -ffreestanding
    -fno-omit-frame-pointer
    -mlittle-endian
    -mstrict-align
    -march=armv8-a
    -isystem ${CMAKE_INSTALL_PREFIX}/include
)

list(APPEND AARCH64_C_FLAGS
    -std=gnu11
)

list(APPEND AARCH64_CXX_FLAGS
    -x c++
    -std=c++17
    # -isystem ${CMAKE_INSTALL_PREFIX}/include/c++/v1
)
