/**
 * Push and pop 'psuedo-op' macros that simplify the ARM syntax to make the below pretty.
 */
.macro  push, xreg1, xreg2
    stp     \xreg1, \xreg2, [sp, #-16]!
.endm

.macro  pop, xreg1, xreg2
    ldp     \xreg1, \xreg2, [sp], #16
.endm

/**
 * Switch execution from EL3 to EL2, preserving the current stack pointer,
 * link register, and general purpose registers
 */
.global _switch_el3_to_el2
_switch_el3_to_el2:
    // Preserve clobbered registers
    push    x0, x1

    // Set ERET return address
    ldr     x0, =_after_el3_to_el2
    msr     elr_el3, x0

    // Preserve stack pointer and link register across exception return
    mov     x0, sp
    mov     x1, lr

    eret    // Fall through
_after_el3_to_el2:
    // Restore the stack pointer, link register, and clobbered registers
    mov     sp, x0
    mov     lr, x1
    pop     x0, x1

    // Return to caller, executing in EL2
    ret

/**
 * Switch execution from EL2 to EL1, preserving the current stack pointer,
 * link register, and general purpose registers
 */
.global _switch_el2_to_el1
_switch_el2_to_el1:
    // Preserve clobbered registers
    push    x0, x1

    // Set ERET return address
    ldr     x0, =_after_el2_to_el1
    msr     elr_el2, x0

    // Preserve stack pointer and link register across exception return
    mov     x0, sp
    mov     x1, lr

    eret    // Fall through
_after_el2_to_el1:
    // Restore the stack pointer, link register, and clobbered registers
    mov     sp, x0
    mov     lr, x1
    pop     x0, x1

    // Return to caller, executing in EL1
    ret

/**
 * Switch execution from EL1 to EL0, preserving the current stack pointer,
 * link register, and general purpose registers
 */
.global _switch_el1_to_el0
_switch_el1_to_el0:
    // Preserve clobbered registers
    push    x0, x1

    // Set ERET return address
    ldr     x0, =_after_el1_to_el0
    msr     elr_el1, x0

    // Preserve stack pointer and link register across exception return
    mov     x0, sp
    mov     x1, lr

    eret    // Fall through
_after_el1_to_el0:
    // Restore the stack pointer, link register, and clobbered registers
    mov     sp, x0
    mov     lr, x1
    pop     x0, x1

    // Return to caller, executing in EL0
    ret

/**
 * Switch execution from EL0 to EL1, preserving the current stack pointer,
 * link register, and general purpose registers
 */
.global _switch_el0_to_el1
_switch_el0_to_el1:
    // Preserve clobbered registers
    push    x0, x1
    push    x2, x3

    // Preserve stack pointer and link register across exception return
    mov     x0, sp
    mov     x1, lr

    // Indicate address for synchronous EL1 exception handler to return to
    ldr     x2, =_after_el0_to_el1

    svc     #0x0    // Fall through
_after_el0_to_el1:
    // Restore the stack pointer, link register, and clobbered registers
    mov     sp, x0
    mov     lr, x1
    pop     x2, x3
    pop     x0, x1

    // Return to caller, executing in EL1
    ret

/**
 * Switch execution from EL1 to EL2, preserving the current stack pointer,
 * link register, and general purpose registers
 */
.global _switch_el1_to_el2
_switch_el1_to_el2:
    // Preserve clobbered registers
    push    x0, x1
    push    x2, x3

    // Preserve stack pointer and link register across exception return
    mov     x0, sp
    mov     x1, lr

    // Indicate address for synchronous EL2 exception handler to return to
    ldr     x2, =_after_el1_to_el2

    hvc     #0x0    // Fall through
_after_el1_to_el2:
    // Restore the stack pointer, link register, and clobbered registers
    mov     sp, x0
    mov     lr, x1
    pop     x2, x3
    pop     x0, x1

    // Return to caller, executing in EL2
    ret

/**
 * Switch execution from EL2 to EL3, preserving the current stack pointer,
 * link register, and general purpose registers
 */
.global _switch_el2_to_el3
_switch_el2_to_el3:
    // Preserve clobbered registers
    push    x0, x1
    push    x2, x3

    // Preserve stack pointer and link register across exception return
    mov     x0, sp
    mov     x1, lr

    // Indicate address for synchronous EL3 exception handler to return to
    ldr     x2, =_after_el2_to_el3

    smc     #0x0    // Fall through
_after_el2_to_el3:
    // Restore the stack pointer, link register, and clobbered registers
    mov     sp, x0
    mov     lr, x1
    pop     x2, x3
    pop     x0, x1

    // Return to caller, executing in EL3
    ret
