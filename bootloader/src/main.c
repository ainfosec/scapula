#include <microlib.h>
#include "bootloader.h"

void bootloader_main(void * fdt)
{
    bfignored(fdt);
    init_bootloader();

    BOOTLOADER_INFO("Hello from EL2");

    switch_to_el1();

    BOOTLOADER_INFO("Hello from EL1");

    panic();
}

