.section ".text"

/*
 * Mock linux-kernel header to trick previous stage bootloader that we're Linux
 */
.globl _header
_header:
        b       scapula_loader_start
        .long   0               // reserved
        .quad   0               // Image load offset from start of RAM
        .quad   0x2000000       // Image size, little endian (32MiB)
        .quad   0               // reserved
        .quad   0               // reserved
        .quad   0               // reserved
        .quad   0               // reserved
        .byte   0x41            // Magic number, "ARM\x64"
        .byte   0x52
        .byte   0x4d
        .byte   0x64
        .word   0                 // reserved

/**
 * Scapula loader start of day code.
 * This is the first code that executes after we're launched by the previous
 * stage bootloader. Used only to set up a sane C environment for 
 * scapula_loader_main()
 *
 */
.global scapula_loader_start
scapula_loader_start:
    // Setup the Scapula OS stack (shared between all exception levels)
    ldr     x1, =scapula_os_stack_end
    mov     x2, sp
    mov     x3, lr
    mov     sp, x1

    // Stash previous stage bootloader
    stp     x0, x1, [sp, #-16]!
    stp     x2, x3, [sp, #-16]!
    stp     x4, x5, [sp, #-16]!
    stp     x6, x7, [sp, #-16]!
    stp     x8, x9, [sp, #-16]!
    stp     x10, x11, [sp, #-16]!
    stp     x12, x11, [sp, #-16]!
    stp     x14, x11, [sp, #-16]!
    stp     x16, x11, [sp, #-16]!
    stp     x18, x11, [sp, #-16]!
    stp     x20, x21, [sp, #-16]!
    stp     x22, x23, [sp, #-16]!
    stp     x24, x25, [sp, #-16]!
    stp     x26, x27, [sp, #-16]!
    stp     x28, x29, [sp, #-16]!
    stp     x30, xzr, [sp, #-16]!

    // Clean out the general purpose registers
    mov     x1, xzr
    mov     x2, xzr
    mov     x3, xzr
    mov     x4, xzr
    mov     x5, xzr
    mov     x6, xzr
    mov     x7, xzr
    mov     x8, xzr
    mov     x9, xzr
    mov     x10, xzr
    mov     x11, xzr
    mov     x12, xzr
    mov     x13, xzr
    mov     x14, xzr
    mov     x15, xzr
    mov     x16, xzr
    mov     x17, xzr
    mov     x18, xzr
    mov     x19, xzr
    mov     x20, xzr
    mov     x21, xzr
    mov     x22, xzr
    mov     x23, xzr
    mov     x24, xzr
    mov     x25, xzr
    mov     x26, xzr
    mov     x27, xzr
    mov     x28, xzr
    mov     x29, xzr
    mov     x30, xzr

    // Run the Scapula loader main routine. This shouldn't return.
    bl      scapula_loader_main

    // On failure, return to previous stage bootloader
    ldp     x30, xzr, [sp], #16
    ldp     x28, x29, [sp], #16
    ldp     x26, x27, [sp], #16
    ldp     x24, x25, [sp], #16
    ldp     x22, x23, [sp], #16
    ldp     x20, x21, [sp], #16
    ldp     x18, x11, [sp], #16
    ldp     x16, x11, [sp], #16
    ldp     x14, x11, [sp], #16
    ldp     x12, x11, [sp], #16
    ldp     x10, x11, [sp], #16
    ldp     x8, x9, [sp], #16
    ldp     x6, x7, [sp], #16
    ldp     x4, x5, [sp], #16
    ldp     x2, x3, [sp], #16
    ldp     x0, x1, [sp], #16

    mov     sp, x2
    mov     lr, x3
    ret
