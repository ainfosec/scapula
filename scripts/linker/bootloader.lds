OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_header)
SECTIONS
{
    /* Load/entry point (where the previous stage bootloader will place us) */
    . = ${BOOTLOADER_START_ADDRESS};

    PROVIDE(bootloader_start = .);
    .text : {
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

    .common : {
        *(COMMON)
    }

    /* Heap */
    . = ALIGN(8);
    PROVIDE(bootloader_heap_start = .);
    . += ${BOOTLOADER_HEAP_SIZE};
    PROVIDE(bootloader_heap_end = .);

    /* Stack */
    . = ALIGN(16);
    PROVIDE(bootloader_stack_start = .);
    . += ${BOOTLOADER_STACK_SIZE};
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

