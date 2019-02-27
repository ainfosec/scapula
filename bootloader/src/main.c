#include "bootloader.h"

void bootloader_main(void * fdt)
{
    init_bootloader();

    panic();
}

