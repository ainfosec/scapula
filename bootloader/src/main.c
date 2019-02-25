#include <microlib.h>
#include "bootloader.h"
#include "switch_exception_level.h"

void bootloader_main(void * fdt)
{
    init_bootloader();

    // Up and down each EL
    BOOTLOADER_INFO("Test: EL2->EL1->EL0->EL1->EL0");
    switch_to_el(1);
    switch_to_el(0);
    switch_to_el(1);
    switch_to_el(2);

    // EL2->EL2
    BOOTLOADER_INFO("Test: EL2->EL2");
    switch_to_el(2);

    // EL1->EL1
    BOOTLOADER_INFO("Test: EL1->EL1");
    switch_to_el(1);
    switch_to_el(1);

    // EL0->EL0
    BOOTLOADER_INFO("Test: EL0->EL0");
    switch_to_el(0);
    switch_to_el(0);

    // EL0->EL2
    BOOTLOADER_INFO("Test: EL0->EL2");
    switch_to_el(2);

    // EL2->EL0
    BOOTLOADER_INFO("Test: EL2->EL0");
    switch_to_el(0);

    panic();
}

