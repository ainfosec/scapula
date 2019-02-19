#ifndef __REGS_H__
#define __REGS_H__

/**
 * Access to system registers.
 */
#define WRITE_SYSREG(sysreg, val, type) \
    asm volatile ("msr        "#sysreg", %0\n" : : "r"((type)(val)))
#define READ_SYSREG(sysreg, val, type) \
    asm volatile ("mrs        %0, "#sysreg"\n" : "=r"((type)(val)))

#define READ_SYSREG_32(sysreg, val)   READ_SYSREG(sysreg, val, uint32_t)
#define WRITE_SYSREG_32(sysreg, val)  WRITE_SYSREG(sysreg, val, uint32_t)

#define READ_SYSREG_64(sysreg, val)   READ_SYSREG(sysreg, val, uint64_t)
#define WRITE_SYSREG_64(sysreg, val)  WRITE_SYSREG(sysreg, val, uint64_t)


/**
 * Returns the system's current Execution Level (EL).
 */
inline static uint32_t get_current_el(void)
{
    uint32_t val;
    READ_SYSREG_32(CurrentEl, val);
    return val >> 2;
}

/**
 * Returns the ID (affinity) of the current core
 */
inline static uint64_t get_core_id(void)
{
    uint64_t reg64 = 0;
    READ_SYSREG_64(mpidr_el1, reg64);
    return reg64 & 0xF;
}

/**
 * Returns the MMU status bit from the SCTLR register.
 */
inline static uint32_t get_el2_mmu_status(void)
{
    uint32_t val;
    READ_SYSREG_32(sctlr_el2, val);
    return val & 1;
}

#endif
