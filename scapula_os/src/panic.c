#include "print.h"
#include "panic.h"

void panic(void)
{
    print_panic();
    while(1);
}
