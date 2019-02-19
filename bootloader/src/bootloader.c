#include <microlib.h>
#include "bootloader.h"
#include "bootloader_common.h"
#include "regs.h"
#include "util.h"

boot_ret_t print_banner()
{
    BOOTLOADER_PRINT("=======================================");
    BOOTLOADER_PRINT(" ___                __ _           _   ");
    BOOTLOADER_PRINT("| _ ) __ _ _ _ ___ / _| |__ _ _ _ | |__");
    BOOTLOADER_PRINT("| _ \\/ _` | '_/ -_)  _| / _` | ' \\| / /");
    BOOTLOADER_PRINT("|___/\\__,_|_| \\___|_| |_\\__,_|_||_|_\\_\\");
    BOOTLOADER_PRINT("     Bootloader  ");
    BOOTLOADER_PRINT("=======================================");

    return BOOT_CONTINUE;
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

    return BOOT_CONTINUE;
}

boot_ret_t init_el2()
{
    // Enable 64-bit execution for EL1
    uint64_t val = 0x80000000;
    WRITE_SYSREG_64(hcr_el2, val);

    return BOOT_CONTINUE;
}

boot_ret_t init_platform_info()
{
    BOOTLOADER_INFO("Initializing platform info");
    BOOTLOADER_ERROR("init_platform_info not yet implemented");
    return BOOT_FAIL;
}

boot_ret_t init_bootloader()
{
    print_banner();
    verify_environment();

    init_el2();

    return BOOT_CONTINUE;
}

boot_ret_t switch_to_el1()
{
    BOOTLOADER_INFO("Switching to EL1...");

    uint32_t el = get_current_el();
    if (el != 2) {
        BOOTLOADER_INFO("Things");
        BOOTLOADER_ERROR("Cannot switch to EL1 from EL%u", el);
        return BOOT_FAIL;
    }
    _switch_to_el1();
    el = get_current_el();
    if (el != 1) {
        BOOTLOADER_INFO("Stuff");
        BOOTLOADER_ERROR("Failed to switch to EL1, currently running at EL%u", el);
        return BOOT_FAIL;
    }

    BOOTLOADER_SUBINFO("...now executing in EL1");
    return BOOT_CONTINUE;
}

// #include <bfelf_loader.h>
boot_ret_t launch_bareflank()
{
    BOOTLOADER_INFO("Launching Bareflank VMM...");
    // const char * target_vmm_start = 0xf00d; // Lookup in image tree
    // const char * target_vmm_end = 0xbeef; // Lookup in image tree
    // uint64_t target_vmm_size = (uint64_t)(target_vmm_end - target_vmm_start);
    //
    // int64_t ret = common_add_module(target_vmm_start, target_vmm_size);
    // if (ret < 0) {
    //     BOOTLOADER_INFO("common_add_module returned %a", ec_to_str(ret));
    //     goto fail;
    // }
    //
    // ret = common_load_vmm();
    // if (ret < 0) {
    //     BOOTLOADER_INFO("common_load_vmm returned %a", ec_to_str(ret));
    //     goto fail;
    // }
    //
    // uint64_t cpus = platform_num_cpus();
    // if (cpus == 0) {
    //     BOOTLOADER_ERROR("No CPUs found!");
    //     goto fail;
    // }
    //
    // BOOTLOADER_INFO("Starting hypervisor...");
    // // for (uint64_t i = 1; i < cpus; i++) {
    // //     status = startup_ap(i);
    // //     if (status != BOOT_CONTINUE) {
    // //         return status;
    // //     }
    // // }
    //
    // // bf_start_hypervisor_on_core(NULL);
    //
    return BOOT_CONTINUE;

fail:
    BOOTLOADER_ERROR("Failed to launch Bareflank VMM (ELF loader not yet implemented)");
    return BOOT_FAIL;
}
