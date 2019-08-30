#include <malloc.h>
#include <stdlib.h>
#include <shoulder/CHeaderGenerator/shoulder.h>

#include "init.h"
#include "print.h"
#include "exception_level.h"

extern uint64_t scapula_os_bss_start;
extern uint64_t scapula_os_bss_end;
extern uint64_t scapula_os_heap_start;
extern uint64_t scapula_os_heap_end;
extern void * el1_vector_table;
extern void * el2_vector_table;
extern void * el3_vector_table;

void init_el3(void)
{
    SCAPULA_INFO("Initializing EL3 environment...");

    // Initialize Interrupt Vector Table
    aarch64_vbar_el3_set((uint64_t)&el3_vector_table);

    // Enable all interrupts
    aarch64_spsr_el3_fieldset_2_d_enable();
    aarch64_spsr_el3_fieldset_2_a_enable();
    aarch64_spsr_el3_fieldset_2_i_enable();
    aarch64_spsr_el3_fieldset_2_f_enable();

    aarch64_scr_el3_rw_enable();
    aarch64_scr_el3_hce_enable();
    aarch64_scr_el3_ns_enable();

    SCAPULA_INFO("...done");
}

void init_el2(void)
{
    SCAPULA_INFO("Initializing EL2 environment...");

    // Initialize Interrupt Vector Table
    aarch64_vbar_el2_set((uint64_t)&el2_vector_table);

    // Enable all interrupts
    aarch64_spsr_el2_fieldset_2_d_enable();
    aarch64_spsr_el2_fieldset_2_a_enable();
    aarch64_spsr_el2_fieldset_2_i_enable();
    aarch64_spsr_el2_fieldset_2_f_enable();

    aarch64_hcr_el2_rw_enable();

    aarch64_sctlr_el2_fieldset_2_m_disable();

    SCAPULA_INFO("...done");
}

void init_el1(void)
{
    SCAPULA_INFO("Initializing EL1 environment...");

    // Initialize Interrupt Vector Table
    aarch64_vbar_el1_set((uint64_t)&el1_vector_table);

    // Enable all interrupts
    aarch64_spsr_el1_fieldset_2_d_enable();
    aarch64_spsr_el1_fieldset_2_a_enable();
    aarch64_spsr_el1_fieldset_2_i_enable();
    aarch64_spsr_el1_fieldset_2_f_enable();

    aarch64_sctlr_el1_lsmaoe_enable();
    aarch64_sctlr_el1_ntlsmd_enable();
    aarch64_sctlr_el1_span_enable();
    aarch64_sctlr_el1_eis_enable();
    aarch64_sctlr_el1_tscxt_enable();
    aarch64_sctlr_el1_i_enable();

    aarch64_cpacr_el1_fpen_set(0x3);

    SCAPULA_INFO("...done");
}

void init_scapula_os(void)
{
    print_banner();

    // Clear BSS
    uint64_t * buffer_start = (uint64_t *)&scapula_os_bss_start;
    uint64_t * buffer_end = (uint64_t *)&scapula_os_bss_end;
    SCAPULA_INFO("Clearing BSS: 0x%016x - 0x%016x", buffer_start, buffer_end);
    while (buffer_start < buffer_end) {
        *buffer_start = 0;
        buffer_start++;
    }

    // Clear and initialize heap memory
    buffer_start = (uint64_t *)&scapula_os_heap_start;
    buffer_end = (uint64_t *)&scapula_os_heap_end;
    SCAPULA_INFO("Initializing Heap: 0x%016x - 0x%016x", buffer_start, buffer_end);
    while (buffer_start < buffer_end) {
        *buffer_start = 0;
        buffer_start++;
    }
    malloc_addblock(&scapula_os_heap_start, &scapula_os_heap_end - &scapula_os_heap_start);
    malloc_init();

    // Initialize g_current_el with whatever exception level Scapula OS
    // is launched in
    uint32_t current_el = aarch64_currentel_el_get();
    set_current_el(current_el);

    switch(current_el) {
        case 3:
            init_el3();
        case 2:
            init_el2();
        case 1:
            init_el1();
    }
}
