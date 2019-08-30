//
// Shoulder
// Copyright (C) 2018 Assured Information Security, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef SCAPULA_AARCH64_ACCESSOR_MACROS_H
#define SCAPULA_AARCH64_ACCESSOR_MACROS_H

// These macros are used to override the default low-level Shoulder macros that
// handle system register access (whole registers, fields, bit-fields, etc).
// The purpose of these macros are to integrate with the Scapula-specific
// interrupt vectors. These macros all preserve the current stack and link
// link register so that execution can continue seemlessly in the event that an
// exception occurs as a result of system register access.

#define SHOULDER_AARCH64_ENCODED_READ_IMPL(instruction_encoding)               \
    uint64_t val;                                                              \
    __asm__ __volatile__(                                                      \
        "stp x0, x1, [sp, #-16]!\n"                                            \
        "stp x2, x9, [sp, #-16]!\n"                                            \
        "mov x0, sp\n"                                                         \
        "mov x1, lr\n"                                                         \
        "mov %[v], xzr\n"                                                      \
        "adr x2, .+8\n"                                                        \
        ".word "#instruction_encoding"\n"                                      \
        "mov %[v], x9\n"                                                       \
        "mov sp, x0\n"                                                         \
        "mov lr, x1\n"                                                         \
        "ldp x2, x9, [sp], #16\n"                                              \
        "ldp x0, x1, [sp], #16\n"                                              \
        : [v] "=r"(val)                                                        \
        :                                                                      \
        : "x0", "x1", "x2", "x9"                                               \
    );                                                                         \
    return val;

#define SHOULDER_AARCH64_ENCODED_WRITE_IMPL(instruction_encoding, val)         \
    __asm__ __volatile__(                                                      \
        "stp x0, x1, [sp, #-16]!\n"                                            \
        "stp x2, x9, [sp, #-16]!\n"                                            \
        "mov x0, sp\n"                                                         \
        "mov x1, lr\n"                                                         \
        "mov x9, %[v]\n"                                                       \
        "adr x2, .+8\n"                                                        \
        ".word "#instruction_encoding"\n"                                      \
        "mov sp, x0\n"                                                         \
        "mov lr, x1\n"                                                         \
        "ldp x2, x9, [sp], #16\n"                                              \
        "ldp x0, x1, [sp], #16\n"                                              \
        :                                                                      \
        : [v] "r"(val)                                                         \
        : "x0", "x1", "x2", "x9"                                               \
    );

#define SHOULDER_AARCH64_LDR_IMPL(addr)                                        \
    SCAPULA_ERROR("SHOULDER_AARCH64_LDR_IMPL not implemented"); \
    panic();

#define SHOULDER_AARCH64_MRS_REGISTER_IMPL(sysreg_mnemonic)                    \
    uint64_t val;                                                              \
    __asm__ __volatile__(                                                      \
        "stp x0, x1, [sp, #-16]!\n"                                            \
        "stp x2, xzr, [sp, #-16]!\n"                                           \
        "mov x0, sp\n"                                                         \
        "mov x1, lr\n"                                                         \
        "mov %[v], xzr\n"                                                      \
        "adr x2, .+8\n"                                                        \
        "mrs %[v], "#sysreg_mnemonic"\n"                                       \
        "mov sp, x0\n"                                                         \
        "mov lr, x1\n"                                                         \
        "ldp x2, xzr, [sp], #16\n"                                             \
        "ldp x0, x1, [sp], #16\n"                                              \
        : [v] "=r"(val)                                                        \
        :                                                                      \
        : "x0", "x1", "x2"                                                     \
    );                                                                         \
    return val;

#define SHOULDER_AARCH64_MSR_IMMEDIATE_IMPL(sysreg_mnemonic, val)              \
    SCAPULA_ERROR("SHOULDER_AARCH64_MSR_IMMEDIATE_IMPL not implemented"); \
    panic();

#define SHOULDER_AARCH64_MSR_REGISTER_IMPL(sysreg_mnemonic, val)               \
    __asm__ __volatile__(                                                      \
        "stp x0, x1, [sp, #-16]!\n"                                            \
        "stp x2, xzr, [sp, #-16]!\n"                                           \
        "mov x0, sp\n"                                                         \
        "mov x1, lr\n"                                                         \
        "adr x2, .+8\n"                                                        \
        "msr "#sysreg_mnemonic", %[v]\n"                                       \
        "mov sp, x0\n"                                                         \
        "mov lr, x1\n"                                                         \
        "ldp x2, xzr, [sp], #16\n"                                             \
        "ldp x0, x1, [sp], #16\n"                                              \
        :                                                                      \
        : [v] "r"(val)                                                         \
        : "x0", "x1", "x2"                                                     \
    );

#define SHOULDER_AARCH64_STR_IMPL(addr, val)                                   \
    SCAPULA_ERROR("SHOULDER_AARCH64_STR_IMPL not implemented"); \
    panic();

#endif
