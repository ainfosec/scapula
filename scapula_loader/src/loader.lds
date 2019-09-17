OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(scapula_loader_start)
SECTIONS
{
    /* Load/entry point (where the previous stage bootloader will place us) */
    . = ${START_ADDRESS};

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
    PROVIDE(scapula_os_bss_start = .);
    .bss (NOLOAD) : {
        *(.bss) . = ALIGN(8);
    }
    PROVIDE(scapula_os_bss_end = .);

    .common : {
        *(COMMON)
    }

    /* Heap */
    . = ALIGN(8);
    PROVIDE(scapula_os_heap_start = .);
    . += ${HEAP_SIZE};
    PROVIDE(scapula_os_heap_end = .);

    /* Stack */
    . = ALIGN(16);
    PROVIDE(scapula_os_stack_start = .);
    . += ${STACK_SIZE};
    PROVIDE(scapula_os_stack_end = .);

    /* Page align the end of the Scapula loader binary */
    . = ALIGN(512);
    PROVIDE(scapula_os_end = .);

    /DISCARD/ : { *(.dynstr*) }
    /DISCARD/ : { *(.dynamic*) }
    /DISCARD/ : { *(.plt*) }
    /DISCARD/ : { *(.interp*) }
    /DISCARD/ : { *(.gnu*) }

}

