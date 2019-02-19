#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include "boot.h"

boot_ret_t print_banner();
boot_ret_t panic();
boot_ret_t verify_environment();
boot_ret_t init_el2();
boot_ret_t switch_to_el1();
boot_ret_t init_platform_info();
boot_ret_t init_bootloader();
boot_ret_t launch_bareflank();

#endif
