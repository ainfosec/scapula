/*
 * Miniture 8250 serial driver that prints a single character synchronously.
 */

#include <stdint.h>
#include "bootloader_config.h"

void _putc(char c)
{
    uintptr_t addr = BOOTLOADER_CONFIG_SERIAL_BASE;

    asm volatile(
      "1:  ldrb  w2, [%[addr], #20]\n"
      "    tbz   w2, #5, 1b\n"
      "    strb  %w[c], [%[addr]]"
      :
      : [c] "r"(c), [addr] "r"(addr)
      : "x2"
    );
}

void _putchar(char c)
{
    _putc(c);
}
