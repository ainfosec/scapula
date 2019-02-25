#ifndef BOOTLOADER_INTERRUPT_VECTORS_H
#define BOOTLOADER_INTERRUPT_VECTORS_H

// Base addresses for interrupt/exception vector tables
// (see interrupt_vectors.s)
extern void * el1_vector_table;
extern void * el2_vector_table;
extern void * el3_vector_table;

// Function pointer type for interrupt vectors to return excecution through
// previous stack pointer and link register
typedef void(*vector_return_fn)(uintptr_t sp, uintptr_t lr);

#endif
