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

#define SHOULDER_AARCH64_SYSL_IMPL(o1, cn, cm, o2)                             \
    uint64_t val;                                                              \
    __asm__ __volatile__(                                                      \
        "stp x0, x1, [sp, #-16]!\n"                                            \
        "stp x2, xzr, [sp, #-16]!\n"                                           \
        "mov x0, sp\n"                                                         \
        "mov x1, lr\n"                                                         \
        "mov %[v], xzr\n"                                                      \
        "adr x2, .+8\n"                                                        \
        "sysl x9, #"#o1", C"#cn", C"#cm", #"#o2"\n"                            \
        "mov %[v], x9\n"                                                       \
        "mov sp, x0\n"                                                         \
        "mov lr, x1\n"                                                         \
        "ldp x2, xzr, [sp], #16\n"                                             \
        "ldp x0, x1, [sp], #16\n"                                              \
        : [v] "=r"(val)                                                        \
        :                                                                      \
        : "x0", "x1", "x2", "x9"                                               \
    );                                                                         \

#define SHOULDER_AARCH64_SYS_IMPL(o1, cn, cm, o2, val)                         \
    __asm__ __volatile__(                                                      \
        "stp x0, x1, [sp, #-16]!\n"                                            \
        "stp x2, xzr, [sp, #-16]!\n"                                           \
        "mov x0, sp\n"                                                         \
        "mov x1, lr\n"                                                         \
        "adr x2, .+8\n"                                                        \
        "sys #"#o1", C"#cn", C"#cm", #"#o2", %[v]\n"                           \
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
