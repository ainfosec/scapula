#ifndef BOOTLOADER_SWITCH_EL_H
#define BOOTLOADER_SWITCH_EL_H

// Change execution to exception level = target_el
boot_ret_t switch_to_el(uint32_t target_el);

#endif
