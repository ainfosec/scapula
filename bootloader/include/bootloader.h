#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <stdint.h>

// Return types and values for bootloader functions
typedef uint64_t boot_ret_t;
#define BOOT_FAIL             ( 0UL )
#define BOOT_SUCCESS          ( 1UL )

// init_bootloader
//
// Initilize the bootloader, verifying the current excecution environment and
// setting up global bootloader state
//
// @return BOOT_SUCCESS if the bootloader was successfully initialized,
// otherwise returns BOOT_FAIL
boot_ret_t init_bootloader(void);

// print_success
//
// Prints a success banner
//
void print_success(void);

// panic
//
// Prints a panic banner, and then halts the CPU
//
// @return does not return, execution will be halted
void panic(void);

// get_current_el
//
// Get the current exception level that the bootloader is currently exceuting at
// Software components should read the current exception level through this
// function because hardware has no way of reporting when the current exception
// level is EL0
//
// @return The current exception level (0 = EL0, 1 = EL1, 2 = EL2, 3 = EL3)
uint32_t get_current_el(void);

// set_current_el
//
// Sets the current exception level that the bootloader is currently exceuting
// at. This function should generally be used only by interrupt and exception
// handlers as a means to indicate to software that the current exception level
// has changed. This function does NOT change the hardware exception level.
//
// @param new_el The new exception level that the bootloader is currently
//      running at (0 = EL0, 1 = EL1, 2 = EL2, 3 = EL3, other values undefined)
void set_current_el(uint32_t new_el);

// TODO: Remove all of the below utilities once shoulder.h is fully integrated
#define WRITE_SYSREG(sysreg, val, type) \
    asm volatile ("msr        "#sysreg", %0\n" : : "r"((type)(val)))
#define READ_SYSREG(sysreg, val, type) \
    asm volatile ("mrs        %0, "#sysreg"\n" : "=r"((type)(val)))

#define READ_SYSREG_32(sysreg, val)   READ_SYSREG(sysreg, val, uint32_t)
#define WRITE_SYSREG_32(sysreg, val)  WRITE_SYSREG(sysreg, val, uint32_t)

#define READ_SYSREG_64(sysreg, val)   READ_SYSREG(sysreg, val, uint64_t)
#define WRITE_SYSREG_64(sysreg, val)  WRITE_SYSREG(sysreg, val, uint64_t)

inline static uint64_t get_core_id(void)
{
    uint64_t reg64 = 0;
    READ_SYSREG_64(mpidr_el1, reg64);
    return reg64 & 0xF;
}

inline static uint32_t get_el2_mmu_status(void)
{
    uint32_t val;
    READ_SYSREG_32(sctlr_el2, val);
    return val & 1;
}

#endif
