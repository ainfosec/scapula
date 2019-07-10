#include "bootloader.h"
#include "microlib.h"
#include "scapula_aarch64_accessor_macros.h"

// #include <scapula/ReadRes0Generator/ReadRes0Generator.h>
// #include <scapula/ReadRes1Generator/ReadRes1Generator.h>
// #include <scapula/ReadImpDefGenerator/ReadImpDefGenerator.h>

void scapula_main(void)
{
    // ReadRes0Generator_run();
    // ReadRes1Generator_run();
    // ReadImpDefGenerator_run();
}

void bootloader_main(void * fdt)
{
    init_bootloader();
    scapula_main();
    print_success();
    while(1);
}
