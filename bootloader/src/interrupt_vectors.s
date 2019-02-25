/*
 * Simple macro to help with generating vector table entries.
 */ 
.macro  ventry  label
    .align  7
    b       \label
.endm

/*
 * Vector table for interrupts/exceptions with destination EL1.
 */
.align  11
.global el1_vector_table;
el1_vector_table:
    ventry to_el1t_synchronous_from_el1    // Synchronous EL1t
    ventry to_el1t_irq_from_el1            // IRQ EL1t
    ventry to_el1t_fiq_from_el1            // FIQ EL1t
    ventry to_el1t_serror_from_el1         // SError EL1t

    ventry to_el1h_synchronous_from_el1    // Synchronous EL1h
    ventry to_el1h_irq_from_el1            // IRQ EL1h
    ventry to_el1h_fiq_from_el1            // FIQ EL1h
    ventry to_el1h_serror_from_el1         // SError EL1h

    ventry to_el1_synchronous_from_aarch64 // Synchronous 64bit from EL0
    ventry to_el1_irq_from_aarch64         // IRQ 64bit from EL0
    ventry to_el1_fiq_from_aarch64         // FIQ 64bit from EL0
    ventry to_el1_serror_from_aarch64      // SError 64bit from EL0

    ventry to_el1_synchronous_from_aarch32 // Synchronous 32bit from EL0
    ventry to_el1_irq_from_aarch32         // IRQ 32bit from EL0
    ventry to_el1_fiq_from_aarch32         // FIQ 32bit from EL0
    ventry to_el1_serror_from_aarch32      // SError 32bit from EL0

/*
 * Vector table for interrupts/exceptions with destination EL2.
 */
.align  11
.global el2_vector_table;
el2_vector_table:
    ventry to_el2t_synchronous_from_el2    // Synchronous EL2t
    ventry to_el2t_irq_from_el2            // IRQ EL2t
    ventry to_el2t_fiq_from_el2            // FIQ EL2t
    ventry to_el2t_serror_from_el2         // SError EL2t

    ventry to_el2h_synchronous_from_el2    // Synchronous EL2h
    ventry to_el2h_irq_from_el2            // IRQ EL2h
    ventry to_el2h_fiq_from_el2            // FIQ EL2h
    ventry to_el2h_serror_from_el2         // SError EL2h

    ventry to_el2_synchronous_from_aarch64 // Synchronous 64bit from EL0/EL1
    ventry to_el2_irq_from_aarch64         // IRQ 64bit from EL0/EL1
    ventry to_el2_fiq_from_aarch64         // FIQ 64bit from EL0/EL1
    ventry to_el2_serror_from_aarch64      // SError 64bit from EL0/EL1

    ventry to_el2_synchronous_from_aarch32 // Synchronous 32bit from EL0/EL1
    ventry to_el2_irq_from_aarch32         // IRQ 32bit from EL0/EL1
    ventry to_el2_fiq_from_aarch32         // FIQ 32bit from EL0/EL1
    ventry to_el2_serror_from_aarch32      // SError 32bit from EL0/EL1

/*
 * Vector table for interrupts/exceptions with destination EL3.
 */
.align  11
.global el3_vector_table;
el3_vector_table:
    ventry to_el3t_synchronous_from_el3    // Synchronous EL3t
    ventry to_el3t_irq_from_el3            // IRQ EL3t
    ventry to_el3t_fiq_from_el3            // FIQ EL3t
    ventry to_el3t_serror_from_el3         // SError EL3t

    ventry to_el3h_synchronous_from_el3    // Synchronous EL3h
    ventry to_el3h_irq_from_el3            // IRQ EL3h
    ventry to_el3h_fiq_from_el3            // FIQ EL3h
    ventry to_el3h_serror_from_el3         // SError EL3h

    ventry to_el3_synchronous_from_aarch64 // Synchronous 64bit from EL0/EL1/EL2
    ventry to_el3_irq_from_aarch64         // IRQ 64bit from EL0/EL1/EL2
    ventry to_el3_fiq_from_aarch64         // FIQ 64bit from EL0/EL1/EL2
    ventry to_el3_serror_from_aarch64      // SError 64bit from EL0/EL1/EL2

    ventry to_el3_synchronous_from_aarch32 // Synchronous 32bit from EL0/EL1/EL2
    ventry to_el3_irq_from_aarch32         // IRQ 32bit from EL0/EL1/EL2
    ventry to_el3_fiq_from_aarch32         // FIQ 32bit from EL0/EL1/EL2
    ventry to_el3_serror_from_aarch32      // SError 32bit from EL0/EL1/EL2
