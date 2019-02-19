OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_header)
SECTIONS
{
    /* Load/entry point (where the previous stage bootloader will place us) */
    /* TODO: Make this configurable through CMake */
    /* . = 0xf0000000; */
    . = 0x80000000;

    . = ALIGN(4);
    .text : {
    PROVIDE(bootloader_start = .);
        *start.o (.text)
        *(.text)
    }

    . = ALIGN(8);
    .rodata : {
        *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
    }

    . = ALIGN(8);
    .data : {
        *(.data)
    }

    /* Uninitialised data */
    . = ALIGN(8);
    PROVIDE(bootloader_bss_start = .);
    .bss (NOLOAD) : {
        *(.bss) . = ALIGN(8);
    }
    PROVIDE(bootloader_bss_end = .);

    /* Bareflank bootloader stack */
    /* TODO: Make this configurable through CMake */
    . = ALIGN(16);
    . += 0x10000; /* 64 KiB stack */
    PROVIDE(bootloader_stack_end = .);

    /* Page align the end of the bootloader */
    . = ALIGN(512);
    PROVIDE(bootloader_end = .);

    /DISCARD/ : { *(.dynstr*) }
    /DISCARD/ : { *(.dynamic*) }
    /DISCARD/ : { *(.plt*) }
    /DISCARD/ : { *(.interp*) }
    /DISCARD/ : { *(.gnu*) }

}

