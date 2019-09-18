target_sources(scapula_os
    PRIVATE
        init.c
        exception_level.s
        exception_level.c
        interrupt_vectors.s
        interrupt_vectors.c
)

if(SERIAL_DEVICE STREQUAL "8250_uart")
    target_sources(scapula_os PRIVATE serial_8250_uart.c)
elseif(SERIAL_DEVICE STREQUAL "pl011_primecell")
    target_sources(scapula_os PRIVATE serial_pl011_primecell.c)
else()
    message(FATAL_ERROR "Serial device not found: ${SERIAL_DEVICE}")
endif()
