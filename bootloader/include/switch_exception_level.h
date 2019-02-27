#ifndef BOOTLOADER_SWITCH_EL_H
#define BOOTLOADER_SWITCH_EL_H

#include "bootloader.h"

// switch_to_el
//
// Change the current exception level (EL) to a new one. Upon returning
// BOOT_SUCCESS, the exception level will be changed to target_el, preserving
// the current stack, link register, and general purpose registers
//
// @param target_el The target exception level to switch to
//      Valid values: 0 (EL0), 1 (EL1), 2 (EL2)
//
// @return BOOT_SUCCESS if the exception level was successfully changed,
//      otherwise BOOT_FAIL
//
// Usage:
//      BOOTLOADER_PRINT("This will print from EL2");
//      switch_to_el(1);
//      BOOTLOADER_PRINT("This will print from EL1");
boot_ret_t switch_to_el(uint32_t target_el);

#endif
