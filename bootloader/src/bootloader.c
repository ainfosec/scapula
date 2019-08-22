#include <microlib.h>
#include <shoulder/CHeaderGenerator/shoulder.h>
#include "bootloader.h"
#include "interrupt_vectors.h"
#include "switch_exception_level.h"

// ----------------------------------------------------------------------------
// Bootloader global state variables and accessors
// ----------------------------------------------------------------------------

// ARMv8-A does not provide a way to detect which excpetion level you are in
// if currently executing in EL0. Therefore, this global var tracks the
// current EL
static volatile uint32_t g_current_el = 0;

uint32_t get_current_el(void)
{
    return g_current_el;
}

void set_current_el(uint32_t new_el)
{
    g_current_el = new_el;
}

// ----------------------------------------------------------------------------
// Bootloader components
// ----------------------------------------------------------------------------
void print_banner(void)
{
    BOOTLOADER_PRINT("=======================================");
    BOOTLOADER_PRINT(" ___                __ _           _   ");
    BOOTLOADER_PRINT("| _ ) __ _ _ _ ___ / _| |__ _ _ _ | |__");
    BOOTLOADER_PRINT("| _ \\/ _` | '_/ -_)  _| / _` | ' \\| / /");
    BOOTLOADER_PRINT("|___/\\__,_|_| \\___|_| |_\\__,_|_||_|_\\_\\");
    BOOTLOADER_PRINT("     Bootloader  ");
    BOOTLOADER_PRINT("=======================================");
}

void print_success(void)
{
    BOOTLOADER_PRINT("---------------------------------------------------------------");
    BOOTLOADER_PRINT(" _______           _______  _______  _______  _______  _______");
    BOOTLOADER_PRINT("(  ____ \\|\\     /|(  ____ \\(  ____ \\(  ____ \\(  ____ \\(  ____ \\");
    BOOTLOADER_PRINT("| (    \\/| )   ( || (    \\/| (    \\/| (    \\/| (    \\/| (    \\/");
    BOOTLOADER_PRINT("| (_____ | |   | || |      | |      | (__    | (_____ | (_____");
    BOOTLOADER_PRINT("(_____  )| |   | || |      | |      |  __)   (_____  )(_____  )");
    BOOTLOADER_PRINT("      ) || |   | || |      | |      | (            ) |      ) |");
    BOOTLOADER_PRINT("/\\____) || (___) || (____/\\| (____/\\| (____/\\/\\____) |/\\____) |");
    BOOTLOADER_PRINT("\\_______)(_______)(_______/(_______/(_______/\\_______)\\_______)");
    BOOTLOADER_PRINT("        The bareflank bootloader exited successfully");
    BOOTLOADER_PRINT("---------------------------------------------------------------");
}

void panic(void)
{
    BOOTLOADER_PRINT("--------------------------------------------------------");
    BOOTLOADER_PRINT("|\\     /||\\     /|       (  ___  )|\\     /|       / )");
    BOOTLOADER_PRINT("| )   ( || )   ( |       | (   ) || )   ( |   _  / /");
    BOOTLOADER_PRINT("| |   | || (___) | _____ | |   | || (___) |  (_)( (");
    BOOTLOADER_PRINT("| |   | ||  ___  |(_____)| |   | ||  ___  |     | |");
    BOOTLOADER_PRINT("| |   | || (   ) |       | |   | || (   ) |   _ ( (");
    BOOTLOADER_PRINT("| (___) || )   ( |       | (___) || )   ( |  (_) \\ \\");
    BOOTLOADER_PRINT("(_______)|/     \\|       (_______)|/     \\|       \\_)");
    BOOTLOADER_PRINT("   The bareflank bootloader terminated unexpectedly");
    BOOTLOADER_PRINT("--------------------------------------------------------");
    while(1);
}

boot_ret_t init_el3(void)
{
    BOOTLOADER_INFO("Initializing EL3 environment...");

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

    BOOTLOADER_INFO("...done");
}

boot_ret_t init_el2(void)
{
    BOOTLOADER_INFO("Initializing EL2 environment...");

    // Initialize Interrupt Vector Table
    aarch64_vbar_el2_set((uint64_t)&el2_vector_table);

    // Enable all interrupts
    aarch64_spsr_el2_fieldset_2_d_enable();
    aarch64_spsr_el2_fieldset_2_a_enable();
    aarch64_spsr_el2_fieldset_2_i_enable();
    aarch64_spsr_el2_fieldset_2_f_enable();

    aarch64_hcr_el2_rw_enable();

    aarch64_sctlr_el2_mmu_disable();

    BOOTLOADER_INFO("...done");
}

boot_ret_t init_el1(void)
{
    BOOTLOADER_INFO("Initializing EL1 environment...");

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

    BOOTLOADER_INFO("...done");
}

boot_ret_t init_bootloader(void)
{
    print_banner();

    // Initialize g_current_el with whatever exception level the bootloader
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

    return BOOT_SUCCESS;
}
