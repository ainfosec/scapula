#include "init.h"
#include "print.h"

void scapula_os_main(void * fdt)
{
    init_scapula_os();
    print_success();
    while(1);
}
