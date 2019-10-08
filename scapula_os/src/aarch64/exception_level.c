#include <stdint.h>
#include "exception_level.h"
#include "print.h"
#include "panic.h"
#include <shoulder/CHeaderGenerator/shoulder.h>

extern void _putc();

// Low-level helpers in switch_exception_level.s
// These are private implmentation details of this file file only
void _switch_el3_to_el2(void);
void _switch_el2_to_el1(void);
void _switch_el1_to_el0(void);
void _switch_el0_to_el1(void);
void _switch_el1_to_el2(void);
void _switch_el2_to_el3(void);

void switch_to_el3(uint64_t current_el);
void switch_to_el2(uint64_t current_el);
void switch_to_el1(uint64_t current_el);
void switch_to_el0(uint64_t current_el);

// ARMv8-A does not provide a way to detect which excpetion level you are in
// if currently executing in EL0. Therefore, this global var tracks the
// current EL
static volatile uint32_t g_current_el = 0xffffffff;

uint32_t get_current_el(void)
{
    return g_current_el;
}

void set_current_el(uint32_t new_el)
{
    g_current_el = new_el;
}

static volatile uint32_t synchronous_interrupt_counter = 0;

uint64_t get_exception_counter(void)
{ return synchronous_interrupt_counter; }

void increment_exception_counter(void)
{ synchronous_interrupt_counter++; }

void reset_exception_counter(void)
{ synchronous_interrupt_counter = 0; }

void switch_to_el3(uint64_t current_el)
{
    switch(current_el){
        case 0:
            switch_to_el1(current_el);
        case 1:
            switch_to_el2(current_el);
        case 2:
            SCAPULA_INFO("Switching EL2->EL3");
            _switch_el2_to_el3();
            SCAPULA_INFO("...done");
        case 3:
            set_current_el(3);
        default:
            return;
    }
}

void switch_to_el2(uint64_t current_el)
{
    uint64_t spsr_el3 = 0;
    uint64_t m_3_0 = 0;

    switch(current_el){
        case 0:
            switch_to_el1(current_el);
        case 1:
            SCAPULA_INFO("Switching EL1->EL2");
            _switch_el1_to_el2();
            SCAPULA_INFO("...done");
        case 2:
            set_current_el(2);
            return;
        case 3:
            aarch64_spsr_el3_fieldset_2_m_3_0__set(0x9);
            SCAPULA_INFO("Switching EL3->EL2");
            _switch_el3_to_el2();
            SCAPULA_INFO("...done");
            set_current_el(2);
        default:
            return;
    }
}

void switch_to_el1(uint64_t current_el)
{
    uint64_t spsr_el2 = 0;
    uint64_t m_3_0 = 0;

    switch(current_el){
        case 3:
            switch_to_el2(current_el);
        case 2:
            aarch64_spsr_el2_fieldset_2_m_3_0__set(0x5);

            SCAPULA_INFO("Switching EL2->EL1");
            _switch_el2_to_el1();
            SCAPULA_INFO("...done");
        case 1:
            set_current_el(1);
            return;
        case 0:
            SCAPULA_INFO("Switching EL0->EL1");
            _switch_el0_to_el1();
            SCAPULA_INFO("...done");
            set_current_el(1);
        default:
            return;
    }
}

void switch_to_el0(uint64_t current_el)
{
    uint32_t spsr_el1 = 0;

    switch(current_el){
        case 3:
            switch_to_el2(current_el);
        case 2:
            switch_to_el1(current_el);
        case 1:
            aarch64_spsr_el1_fieldset_2_m_3_0__set(0x0);

            SCAPULA_INFO("Switching EL1->EL0");
            _switch_el1_to_el0();
            SCAPULA_INFO("...done");
        case 0:
            set_current_el(0);
        default:
            return;
    }
}

void switch_to_el(uint32_t target_el)
{
    if(target_el > 3) {
        SCAPULA_ERROR("Invalid target exception level: %u", target_el);
        panic();
    }

    uint32_t current_el = get_current_el();
    if(current_el > 3) {
        SCAPULA_ERROR("Invalid current exception level: %u", current_el);
        panic();
    }

    switch(target_el){
        case 0:
            switch_to_el0(current_el);
            break;
        case 1:
            switch_to_el1(current_el);
            break;
        case 2:
            switch_to_el2(current_el);
            break;
        case 3:
            switch_to_el3(current_el);
            break;
        default:
            break;
            panic();
    }
}
