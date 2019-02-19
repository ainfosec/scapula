#ifndef BOOTLOADER_UTIL_H
#define BOOTLOADER_UTIL_H

// Switch to EL1 at the current link register, preserving the current
// stack and all general-purpose registers.
void _switch_to_el1(void);

#endif
