/*
 * PrimeCell UART PL011 serial driver that prints a single character synchronously
 */

#include <stdint.h>
#include "cmake_config.h"

void _putc(char c)
{
    uintptr_t addr = CMAKE_CONFIG_SERIAL_BASE;

    asm volatile(
      "strb %w[c], [%[addr]]"
      :
      : [c] "r"(c), [addr] "r"(addr)
      :
    );
}

void _putchar(char c)
{
    _putc(c);
}
