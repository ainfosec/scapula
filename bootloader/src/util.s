/**
 * Push and pop 'psuedo-op' macros that simplify the ARM syntax to make the below pretty.
 */
.macro  push, xreg1, xreg2
    stp     \xreg1, \xreg2, [sp, #-16]!
.endm

.macro  pop, xreg1, xreg2
    ldp     \xreg1, \xreg2, [sp], #16
.endm

.global _switch_to_el1
_switch_to_el1:
    push    x0, x1

    ldr     x0, =_in_el1
    msr     elr_el2, x0

    // Set target PSTATE = EL1 with DAIF masked
    mov     x0, #0x3c5
    msr     spsr_el2, x0

    // Preserve EL2 stack pointer and link register across exception return
    mov     x0, sp
    mov     x1, lr 

    eret    // Fall through
_in_el1:
    // Restore the EL2 stack pointer, link register, and clobbered registers
    mov     sp, x0
    mov     lr, x1
    pop     x0, x1

    // Return to caller, executing in EL1
    ret

/*
 * Handoff from bareflank bootloader to Linux
 *
 * x0: The location of the Linux kernel
 * x1: The device tree to pass to Linux
 */
.global _launch_linux
_launch_linux:
    mov     x4, x0

    mov     x0, x1
    mov     x1, #0
    mov     x2, #0
    mov     x3, #0

    blr      x4

    // We shouldn't ever reach here; trap.
1:  b       1b

