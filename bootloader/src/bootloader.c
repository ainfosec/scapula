#include <microlib.h>
#include <shoulder/CHeaderGenerator/shoulder.h>
#include "bootloader.h"
#include "interrupt_vectors.h"

boot_ret_t print_banner()
{
    BOOTLOADER_PRINT("=======================================");
    BOOTLOADER_PRINT(" ___                __ _           _   ");
    BOOTLOADER_PRINT("| _ ) __ _ _ _ ___ / _| |__ _ _ _ | |__");
    BOOTLOADER_PRINT("| _ \\/ _` | '_/ -_)  _| / _` | ' \\| / /");
    BOOTLOADER_PRINT("|___/\\__,_|_| \\___|_| |_\\__,_|_||_|_\\_\\");
    BOOTLOADER_PRINT("     Bootloader  ");
    BOOTLOADER_PRINT("=======================================");

    return BOOT_SUCCESS;
}

boot_ret_t panic()
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

boot_ret_t verify_environment()
{
    BOOTLOADER_INFO("Verifying environment");

    uint32_t el = get_current_el();
    BOOTLOADER_SUBINFO("executing in EL%u", el);
    if (el != 2) {
        BOOTLOADER_ERROR("Bareflank must be launched from EL2, aborting");
        return BOOT_FAIL;
    }

    uint32_t mmu_status = get_el2_mmu_status();
    if (mmu_status) {
        BOOTLOADER_ERROR("MMU is already on, aborting");
        return BOOT_FAIL;
    }
    BOOTLOADER_SUBINFO("MMU is off");

    return BOOT_SUCCESS;
}

boot_ret_t init_bootloader()
{
    print_banner();

    boot_ret_t status = verify_environment();
    if (status != BOOT_SUCCESS) return status;

    // Enable 64-bit execution for EL1/EL0
    // TODO: Shoulder doesn't generate accessors for HCR_EL2 for some reason
    uint64_t val = 0x80000000;
    WRITE_SYSREG_64(hcr_el2, val);

    // Setup interrupt/exception vectors for EL2/EL1
    aarch64_vbar_el1_set((uint64_t)&el1_vector_table);
    aarch64_vbar_el2_set((uint64_t)&el2_vector_table);

    return BOOT_SUCCESS;
}
