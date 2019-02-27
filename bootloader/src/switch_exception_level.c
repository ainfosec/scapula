#include <stdint.h>
#include "switch_exception_level.h"
#include "microlib.h"
#include <shoulder/CHeaderGenerator/shoulder.h>

// Low-level helpers in switch_exception_level.s
// These are private implmentation details of this file file only
void _switch_el2_to_el1(void);
void _switch_el1_to_el0(void);
void _switch_el1_to_el2(void);
void _switch_el0_to_el1(void);

boot_ret_t switch_to_el2(uint64_t current_el)
{
    switch(current_el){
        case 0:
            BOOTLOADER_INFO("Switching EL0->EL1");
            _switch_el0_to_el1();
            BOOTLOADER_INFO("...done");
        case 1:
            BOOTLOADER_INFO("Switching EL1->EL2");
            _switch_el1_to_el2();
            BOOTLOADER_INFO("...done");
        case 2:
            set_current_el(2);
            return BOOT_SUCCESS;
        default:
            return BOOT_FAIL;
    }
}

boot_ret_t switch_to_el1(uint64_t current_el)
{
    uint32_t spsr_el2 = 0;
    uint32_t m_3_0 = 0;

    switch(current_el){
        case 2:
            spsr_el2 = aarch64_spsr_el2_get();
            m_3_0 = aarch64_spsr_el2_m_3_0__get();
            m_3_0 = (m_3_0 & 0x1) | 0x4;
            spsr_el2 = aarch64_spsr_el2_m_3_0__set_val(spsr_el2, m_3_0);
            aarch64_spsr_el2_set(spsr_el2);
            BOOTLOADER_INFO("Switching EL2->EL1");
            _switch_el2_to_el1();
            BOOTLOADER_INFO("...done");
        case 1:
            set_current_el(1);
            return BOOT_SUCCESS;
        case 0:
            BOOTLOADER_INFO("Switching EL0->EL1");
            _switch_el0_to_el1();
            BOOTLOADER_INFO("...done");
            set_current_el(1);
            return BOOT_SUCCESS;
        default:
            return BOOT_FAIL;
    }
}

boot_ret_t switch_to_el0(uint64_t current_el)
{
    uint32_t spsr_el1 = 0;

    switch(current_el){
        case 2:
            switch_to_el1(current_el);
        case 1:
            spsr_el1 = aarch64_spsr_el1_get();
            aarch64_spsr_el1_m_3_0__set_val(spsr_el1, 0);
            aarch64_spsr_el1_set(spsr_el1);
            BOOTLOADER_INFO("Switching EL1->EL0");
            _switch_el1_to_el0();
            BOOTLOADER_INFO("...done");
        case 0:
            set_current_el(0);
            return BOOT_SUCCESS;
        default:
            return BOOT_FAIL;
    }
}

boot_ret_t switch_to_el(uint32_t target_el)
{
    if(target_el > 2) {
        BOOTLOADER_ERROR("Invalid target excpetion level: %u", target_el);
        return BOOT_FAIL;
    }

    uint32_t current_el = get_current_el();
    if(current_el > 2) {
        BOOTLOADER_ERROR("Invalid current excpetion level: %u", current_el);
        return BOOT_FAIL;
    }

    switch(target_el){
        case 0:
            switch_to_el0(current_el);
            break;
        case 1:
            switch_to_el1(current_el);
            break;
        case 2:
            switch_to_el2(current_el);
            break;
        default:
            return BOOT_FAIL;
    }

    return BOOT_SUCCESS;
}
