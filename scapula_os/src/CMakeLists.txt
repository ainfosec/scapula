cmake_minimum_required(VERSION 3.6)
project(scapula_os C ASM-ATT)
include(${SCAPULA_SOURCE_CMAKE_DIR}/init_subproject.cmake)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../include)

list(APPEND SRC_FILES
    start.s
    main.c
    init.c
    panic.c
    interrupt_vectors.s
    interrupt_vectors.c
    exception_level.s
    exception_level.c
)

if(SERIAL_DEVICE STREQUAL "8250_uart")
    list(APPEND SRC_FILES
        serial_8250_uart.c
    )
elseif(SERIAL_DEVICE STREQUAL "pl011_primecell")
    list(APPEND SRC_FILES
        serial_pl011_primecell.c
    )
else()
    message(FATAL_ERROR "Serial device not found: ${SERIAL_DEVICE}")
endif()

add_executable(scapula_os ${SRC_FILES})

target_compile_definitions(scapula_os
    PRIVATE CMAKE_CONFIG_SERIAL_BASE=${SERIAL_BASE}
)

if(NOT START_ADDRESS)
    message(FATAL_ERROR "Scapula OS start address has not been set")
endif()
configure_file(
    "${SCAPULA_SOURCE_ROOT_DIR}/scripts/linker/scapula_os.lds"
    "${CMAKE_CURRENT_BINARY_DIR}/scapula_os.lds"
)

set(LINKER_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/scapula_os.lds")
set_target_properties(scapula_os PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
target_link_libraries(scapula_os -T ${LINKER_SCRIPT})

list(APPEND LIBRARIES
    ${CMAKE_INSTALL_PREFIX}/lib/libfdt.a
    ${CMAKE_INSTALL_PREFIX}/lib/libc.a
    ${CMAKE_INSTALL_PREFIX}/lib/libprintf.a
    ${CMAKE_INSTALL_PREFIX}/lib/libmemory_freelist.a
)

target_link_libraries(scapula_os ${LIBRARIES})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/scapula_os DESTINATION bin)
install(DIRECTORY ../include/ DESTINATION include/scapula_os)
