#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include <stdint.h>

typedef uint64_t boot_ret_t;

#define BOOT_FAIL             ( 0UL )
#define BOOT_SUCCESS          ( 1UL )

boot_ret_t print_banner();
boot_ret_t panic();
boot_ret_t init_bootloader();

// TODO: Remove all of the below utilities once shoulder.h is fully integrated
#define WRITE_SYSREG(sysreg, val, type) \
    asm volatile ("msr        "#sysreg", %0\n" : : "r"((type)(val)))
#define READ_SYSREG(sysreg, val, type) \
    asm volatile ("mrs        %0, "#sysreg"\n" : "=r"((type)(val)))

#define READ_SYSREG_32(sysreg, val)   READ_SYSREG(sysreg, val, uint32_t)
#define WRITE_SYSREG_32(sysreg, val)  WRITE_SYSREG(sysreg, val, uint32_t)

#define READ_SYSREG_64(sysreg, val)   READ_SYSREG(sysreg, val, uint64_t)
#define WRITE_SYSREG_64(sysreg, val)  WRITE_SYSREG(sysreg, val, uint64_t)


inline static uint32_t get_current_el(void)
{
    uint32_t val;
    READ_SYSREG_32(CurrentEl, val);
    return val >> 2;
}

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
