#include <stdint.h>
#include "exception_level.h"
#include "print.h"
#include "panic.h"

// Function pointer type for interrupt vectors to return excecution through
// previous stack pointer and link register
typedef void(*vector_return_fn)(uintptr_t sp, uintptr_t lr);

// ----------------------------------------------------------------------------
// Vectors destined to EL1 (thread mode) from EL1
// ----------------------------------------------------------------------------
void to_el1t_synchronous_from_el1(uintptr_t sp, uintptr_t lr, uintptr_t ret)
{
    VECTOR_PRINT("to_el1t_synchronous_from_el1");
    set_current_el(1);
    vector_return_fn ret_fn = (vector_return_fn)ret;
    ret_fn(sp, lr);
}

void to_el1t_irq_from_el1(void)
{
    VECTOR_PRINT("to_el1t_irq_from_el1");
    panic();
}

void to_el1t_fiq_from_el1(void)
{
    VECTOR_PRINT("to_el1t_fiq_from_el1");
    panic();
}

void to_el1t_serror_from_el1(void)
{
    VECTOR_PRINT("to_el1t_serror_from_el1");
    panic();
}

// ----------------------------------------------------------------------------
// Vectors destined to EL1 (handler mode) from EL1
// ----------------------------------------------------------------------------
void to_el1h_synchronous_from_el1(uintptr_t sp, uintptr_t lr, uintptr_t ret)
{
    VECTOR_PRINT("to_el1h_synchronous_from_el1");
    set_current_el(1);
    vector_return_fn ret_fn = (vector_return_fn)ret;
    ret_fn(sp, lr);
}

void to_el1h_irq_from_el1(void)
{
    VECTOR_PRINT("to_el1h_irq_from_el1");
    panic();
}

void to_el1h_fiq_from_el1(void)
{
    VECTOR_PRINT("to_el1h_fiq_from_el1");
    panic();
}

void to_el1h_serror_from_el1(void)
{
    VECTOR_PRINT("to_el1h_serror_from_el1");
    panic();
}

// ----------------------------------------------------------------------------
// Vectors destined to EL1 from EL0 in aarch64 state
// ----------------------------------------------------------------------------
void to_el1_synchronous_from_aarch64(uintptr_t sp, uintptr_t lr, uintptr_t ret)
{
    VECTOR_PRINT("to_el1_synchronous_from_aarch64");
    set_current_el(1);
    vector_return_fn ret_fn = (vector_return_fn)ret;
    ret_fn(sp, lr);
}

void to_el1_irq_from_aarch64(void)
{
    VECTOR_PRINT("to_el1_irq_from_aarch64");
    panic();
}

void to_el1_fiq_from_aarch64(void)
{
    VECTOR_PRINT("to_el1_fiq_from_aarch64");
    panic();
}

void to_el1_serror_from_aarch64(void)
{
    VECTOR_PRINT("to_el1_serror_from_aarch64");
    panic();
}

// ----------------------------------------------------------------------------
// Vectors destined to EL1 from EL0 in aarch32 state
// ----------------------------------------------------------------------------
void to_el1_synchronous_from_aarch32(uintptr_t sp, uintptr_t lr, uintptr_t ret)
{
    VECTOR_PRINT("to_el1_synchronous_from_aarch32");
    set_current_el(1);
    vector_return_fn ret_fn = (vector_return_fn)ret;
    ret_fn(sp, lr);
}

void to_el1_irq_from_aarch32(void)
{
    VECTOR_PRINT("to_el1_irq_from_aarch32");
    panic();
}

void to_el1_fiq_from_aarch32(void)
{
    VECTOR_PRINT("to_el1_fiq_from_aarch32");
    panic();
}

void to_el1_serror_from_aarch32(void)
{
    VECTOR_PRINT("to_el1_serror_from_aarch32");
    panic();
}

// ----------------------------------------------------------------------------
// Vectors destined to EL2 (thread mode) from EL2
// ----------------------------------------------------------------------------
void to_el2t_synchronous_from_el2(uintptr_t sp, uintptr_t lr, uintptr_t ret)
{
    VECTOR_PRINT("to_el2t_synchronous_from_el2");
    set_current_el(2);
    vector_return_fn ret_fn = (vector_return_fn)ret;
    ret_fn(sp, lr);
}

void to_el2t_irq_from_el2(void)
{
    VECTOR_PRINT("to_el2t_irq_from_el2");
    panic();
}

void to_el2t_fiq_from_el2(void)
{
    VECTOR_PRINT("to_el2t_fiq_from_el2");
    panic();
}

void to_el2t_serror_from_el2(void)
{
    VECTOR_PRINT("to_el2t_serror_from_el2");
    panic();
}

// ----------------------------------------------------------------------------
// Vectors destined to EL2 (handler mode) from EL2
// ----------------------------------------------------------------------------
void to_el2h_synchronous_from_el2(uintptr_t sp, uintptr_t lr, uintptr_t ret)
{
    VECTOR_PRINT("to_el2h_synchronous_from_el2");
    set_current_el(2);
    vector_return_fn ret_fn = (vector_return_fn)ret;
    ret_fn(sp, lr);
}

void to_el2h_irq_from_el2(void)
{
    VECTOR_PRINT("to_el2h_irq_from_el2");
    panic();
}

void to_el2h_fiq_from_el2(void)
{
    VECTOR_PRINT("to_el2h_fiq_from_el2");
    panic();
}

void to_el2h_serror_from_el2(void)
{
    VECTOR_PRINT("to_el2h_serror_from_el2");
    panic();
}

// ----------------------------------------------------------------------------
// Vectors destined to EL2 from EL0/EL1 in aarch64 state
// ----------------------------------------------------------------------------
void to_el2_synchronous_from_aarch64(uintptr_t sp, uintptr_t lr, uintptr_t ret)
{
    VECTOR_PRINT("to_el2_synchronous_from_aarch64");
    set_current_el(2);
    vector_return_fn ret_fn = (vector_return_fn)ret;
    ret_fn(sp, lr);
}

void to_el2_irq_from_aarch64(void)
{
    VECTOR_PRINT("to_el2_irq_from_aarch64");
    panic();
}

void to_el2_fiq_from_aarch64(void)
{
    VECTOR_PRINT("to_el2_fiq_from_aarch64");
    panic();
}

void to_el2_serror_from_aarch64(void)
{
    VECTOR_PRINT("to_el2_serror_from_aarch64");
    panic();
}

// ----------------------------------------------------------------------------
// Vectors destined to EL2 from EL0/EL1 in aarch32 state
// ----------------------------------------------------------------------------
void to_el2_synchronous_from_aarch32(uintptr_t sp, uintptr_t lr, uintptr_t ret)
{
    VECTOR_PRINT("to_el2_synchronous_from_aarch32");
    set_current_el(2);
    vector_return_fn ret_fn = (vector_return_fn)ret;
    ret_fn(sp, lr);
}

void to_el2_irq_from_aarch32(void)
{
    VECTOR_PRINT("to_el2_irq_from_aarch32");
    panic();
}

void to_el2_fiq_from_aarch32(void)
{
    VECTOR_PRINT("to_el2_fiq_from_aarch32");
    panic();
}

void to_el2_serror_from_aarch32(void)
{
    VECTOR_PRINT("to_el2_serror_from_aarch32");
    panic();
}

// ----------------------------------------------------------------------------
// Vectors destined to EL3 (thread mode) from EL3
// ----------------------------------------------------------------------------
void to_el3t_synchronous_from_el3(uintptr_t sp, uintptr_t lr, uintptr_t ret)
{
    VECTOR_PRINT("to_el3t_synchronous_from_el3");
    set_current_el(3);
    vector_return_fn ret_fn = (vector_return_fn)ret;
    ret_fn(sp, lr);
}

void to_el3t_irq_from_el3(void)
{
    VECTOR_PRINT("to_el3t_irq_from_el3");
    panic();
}

void to_el3t_fiq_from_el3(void)
{
    VECTOR_PRINT("to_el3t_fiq_from_el3");
    panic();
}

void to_el3t_serror_from_el3(void)
{
    VECTOR_PRINT("to_el3t_serror_from_el3");
    panic();
}

// ----------------------------------------------------------------------------
// Vectors destined to EL3 (handler mode) from EL3
// ----------------------------------------------------------------------------
void to_el3h_synchronous_from_el3(uintptr_t sp, uintptr_t lr, uintptr_t ret)
{
    VECTOR_PRINT("to_el3h_synchronous_from_el3");
    set_current_el(3);
    vector_return_fn ret_fn = (vector_return_fn)ret;
    ret_fn(sp, lr);
}

void to_el3h_irq_from_el3(void)
{
    VECTOR_PRINT("to_el3h_irq_from_el3");
    panic();
}

void to_el3h_fiq_from_el3(void)
{
    VECTOR_PRINT("to_el3h_fiq_from_el3");
    panic();
}

void to_el3h_serror_from_el3(void)
{
    VECTOR_PRINT("to_el3h_serror_from_el3");
    panic();
}

// ----------------------------------------------------------------------------
// Vectors destined to EL3 from EL0/EL1/EL2 in aarch64 state
// ----------------------------------------------------------------------------
void to_el3_synchronous_from_aarch64(uintptr_t sp, uintptr_t lr, uintptr_t ret)
{
    VECTOR_PRINT("to_el3_synchronous_from_aarch64");
    set_current_el(3);
    vector_return_fn ret_fn = (vector_return_fn)ret;
    ret_fn(sp, lr);
}

void to_el3_irq_from_aarch64(void)
{
    VECTOR_PRINT("to_el3_irq_from_aarch64");
    panic();
}

void to_el3_fiq_from_aarch64(void)
{
    VECTOR_PRINT("to_el3_fiq_from_aarch64");
    panic();
}

void to_el3_serror_from_aarch64(void)
{
    VECTOR_PRINT("to_el3_serror_from_aarch64");
    panic();
}

// ----------------------------------------------------------------------------
// Vectors destined to EL3 from EL0/EL1/EL2 in aarch32 state
// ----------------------------------------------------------------------------
void to_el3_synchronous_from_aarch32(uintptr_t sp, uintptr_t lr, uintptr_t ret)
{
    VECTOR_PRINT("to_el3_synchronous_from_aarch32");
    set_current_el(3);
    vector_return_fn ret_fn = (vector_return_fn)ret;
    ret_fn(sp, lr);
}

void to_el3_irq_from_aarch32(void)
{
    VECTOR_PRINT("to_el3_irq_from_aarch32");
    panic();
}

void to_el3_fiq_from_aarch32(void)
{
    VECTOR_PRINT("to_el3_fiq_from_aarch32");
    panic();
}

void to_el3_serror_from_aarch32(void)
{
    VECTOR_PRINT("to_el3_serror_from_aarch32");
    panic();
}
