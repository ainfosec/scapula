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

#ifndef SCAPULA_AARCH64_REGS_H
#define SCAPULA_AARCH64_REGS_H

// These macros are used to override the default low-level Shoulder macros that
// handle system register access (whole registers, fields, bit-fields, etc).
// The purpose of these macros are to integrate with the Scapula-specific
// interrupt vectors. These macros all preserve the current stack and link
// link register so that execution can continue seemlessly in the event that an
// exception occurs as a result of system register access.

#define GET_SYSREG(sysreg, dest)                                               \
    __asm__ __volatile__(                                                      \
        "stp x0, x1, [sp, #-16]!\n"                                            \
        "stp x2, xzr, [sp, #-16]!\n"                                           \
        "mov x0, sp\n"                                                         \
        "mov x1, lr\n"                                                         \
        "mov %[d], xzr\n"                                                      \
        "adr x2, .+8\n"                                                        \
        "mrs %[d], "#sysreg"\n"                                                \
        "mov sp, x0\n"                                                         \
        "mov lr, x1\n"                                                         \
        "ldp x2, xzr, [sp], #16\n"                                             \
        "ldp x0, x1, [sp], #16\n"                                              \
        : [d] "=r"(dest)                                                       \
        :                                                                      \
        : "x0", "x1", "x2"                                                     \
    )                                                                          \

#define GET_SYSREG_FIELD(sysreg, dest, mask, lsb)                              \
    __asm__ __volatile__(                                                      \
        "stp x0, x1, [sp, #-16]!\n"                                            \
        "stp x2, xzr, [sp, #-16]!\n"                                           \
        "mov x0, sp\n"                                                         \
        "mov x1, lr\n"                                                         \
        "mov %[d], xzr\n"                                                      \
        "adr x2, .+8\n"                                                        \
        "mrs %[d], "#sysreg"\n"                                                \
        "and %[d], %[d], %[m]\n"                                               \
        "lsr %[d], %[d], #"#lsb"\n"                                            \
        "mov sp, x0\n"                                                         \
        "mov lr, x1\n"                                                         \
        "ldp x2, xzr, [sp], #16\n"                                             \
        "ldp x0, x1, [sp], #16\n"                                              \
        : [d] "+r" (dest)                                                      \
        : [m] "r" (mask)                                                       \
        : "x0", "x1", "x2"                                                     \
    )                                                                          \

#define SET_SYSREG_BY_VALUE(sysreg, val)                                       \
    __asm__ __volatile__(                                                      \
        "stp x0, x1, [sp, #-16]!\n"                                            \
        "stp x2, xzr, [sp, #-16]!\n"                                           \
        "mov x0, sp\n"                                                         \
        "mov x1, lr\n"                                                         \
        "adr x2, .+8\n"                                                        \
        "msr "#sysreg", %[v]\n"                                                \
        "mov sp, x0\n"                                                         \
        "mov lr, x1\n"                                                         \
        "ldp x2, xzr, [sp], #16\n"                                             \
        "ldp x0, x1, [sp], #16\n"                                              \
        :                                                                      \
        : [v] "r"(val)                                                         \
        : "x0", "x1", "x2"                                                     \
    )

#define SET_SYSREG_BITS_BY_VALUE(sysreg, new_val, old_val, mask, lsb)          \
    __asm__ __volatile__(                                                      \
        "stp x0, x1, [sp, #-16]!\n"                                            \
        "stp x2, xzr, [sp, #-16]!\n"                                           \
        "mov x0, sp\n"                                                         \
        "mov x1, lr\n"                                                         \
        "lsl %[nv], %[nv], #"#lsb"\n"                                          \
        "and %[nv], %[nv], %[m]\n"                                             \
        "mvn %[m], %[m]\n"                                                     \
        "adr x2, .+20\n"                                                       \
        "mrs %[ov], "#sysreg"\n"                                               \
        "and %[ov], %[ov], %[m]\n"                                             \
        "orr %[nv], %[nv], %[ov]\n"                                            \
        "msr "#sysreg", %[nv]\n"                                               \
        "mov sp, x0\n"                                                         \
        "mov lr, x1\n"                                                         \
        "ldp x2, xzr, [sp], #16\n"                                             \
        "ldp x0, x1, [sp], #16\n"                                              \
        : [nv] "+r" (new_val), [ov] "+r" (old_val), [m] "+r" (mask)            \
        :                                                                      \
        : "x0", "x1", "x2"                                                     \
    )

#define SET_SYSREG_BITS_BY_MASK(sysreg, val, mask)                             \
    __asm__ __volatile__(                                                      \
        "stp x0, x1, [sp, #-16]!\n"                                            \
        "stp x2, xzr, [sp, #-16]!\n"                                           \
        "mov x0, sp\n"                                                         \
        "mov x1, lr\n"                                                         \
        "adr x2, .+16\n"                                                       \
        "mrs %[v], "#sysreg"\n"                                                \
        "orr %[v], %[v], %[m]\n"                                               \
        "msr "#sysreg", %[v]\n"                                                \
        "mov sp, x0\n"                                                         \
        "mov lr, x1\n"                                                         \
        "ldp x2, xzr, [sp], #16\n"                                             \
        "ldp x0, x1, [sp], #16\n"                                              \
        : [v] "+r" (val)                                                       \
        : [m] "r" (mask)                                                       \
        : "x0", "x1", "x2"                                                     \
    )

#define CLEAR_SYSREG_BITS_BY_MASK(sysreg, val, mask)                           \
    __asm__ __volatile__(                                                      \
        "stp x0, x1, [sp, #-16]!\n"                                            \
        "stp x2, xzr, [sp, #-16]!\n"                                           \
        "mov x0, sp\n"                                                         \
        "mov x1, lr\n"                                                         \
        "adr x2, .+20\n"                                                       \
        "mrs %[v], "#sysreg"\n"                                                \
        "mvn %[m], %[m]\n"                                                     \
        "and %[v], %[v], %[m]\n"                                               \
        "msr "#sysreg", %[v]\n"                                                \
        "mov sp, x0\n"                                                         \
        "mov lr, x1\n"                                                         \
        "ldp x2, xzr, [sp], #16\n"                                             \
        "ldp x0, x1, [sp], #16\n"                                              \
        : [v] "+r" (val), [m] "+r" (mask)                                      \
        :                                                                      \
        : "x0", "x1", "x2"                                                     \
    )

#define IS_SYSREG_BIT_ENABLED(sysreg, result, lsb)                             \
    __asm__ __volatile__(                                                      \
        "stp x0, x1, [sp, #-16]!\n"                                            \
        "stp x2, xzr, [sp, #-16]!\n"                                           \
        "mov x0, sp\n"                                                         \
        "mov x1, lr\n"                                                         \
        "adr x2, .+20\n"                                                       \
        "mrs %[res], "#sysreg"\n"                                              \
        "lsr %[res], %[res], #"#lsb"\n"                                        \
        "and %[res], %[res], #1\n"                                             \
        "mov sp, x0\n"                                                         \
        "mov lr, x1\n"                                                         \
        "ldp x2, xzr, [sp], #16\n"                                             \
        "ldp x0, x1, [sp], #16\n"                                              \
        : [res] "+r" (result)                                                  \
        :                                                                      \
        : "x0", "x1", "x2"                                                     \
    )

#define IS_SYSREG_BIT_DISABLED(sysreg, result, lsb)                            \
    __asm__ __volatile__(                                                      \
        "stp x0, x1, [sp, #-16]!\n"                                            \
        "stp x2, xzr, [sp, #-16]!\n"                                           \
        "mov x0, sp\n"                                                         \
        "mov x1, lr\n"                                                         \
        "adr x2, .+20\n"                                                       \
        "mrs %[res], "#sysreg"\n"                                              \
        "lsr %[res], %[res], #"#lsb"\n"                                        \
        "mvn %[res], %[res]\n"                                                 \
        "and %[res], %[res], #1\n"                                             \
        "mov sp, x0\n"                                                         \
        "mov lr, x1\n"                                                         \
        "ldp x2, xzr, [sp], #16\n"                                             \
        "ldp x0, x1, [sp], #16\n"                                              \
        : [res] "+r" (result)                                                  \
        :                                                                      \
        : "x0", "x1", "x2"                                                     \
    )

#endif
