cmake_minimum_required(VERSION 3.6)
project(scapula_loader C ASM-ATT)
include(${SCAPULA_SOURCE_CMAKE_DIR}/init_subproject.cmake)

if(NOT START_ADDRESS)
    message(FATAL_ERROR "Scapula loader start address has not been set")
endif()

# ------------------------------------------------------------------------------
# Scapula loader (.elf)
# ------------------------------------------------------------------------------

add_executable(scapula_loader
    src/start.s
    src/main.c
    src/run_test_cases.c
)

target_include_directories(scapula_loader
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../scapula_os/include
)

target_compile_definitions(scapula_loader
    PRIVATE LOG_LEVEL=${LOG_LEVEL}
)

set(CONFIGURED_LINKER_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/loader.lds")
configure_file(src/loader.lds ${CONFIGURED_LINKER_SCRIPT})

set_target_properties(scapula_loader
    PROPERTIES LINK_DEPENDS ${CONFIGURED_LINKER_SCRIPT}
)

target_link_libraries(scapula_loader
    ${CMAKE_INSTALL_PREFIX}/lib/libfdt.a
    ${CMAKE_INSTALL_PREFIX}/lib/libc.a
    --whole-archive ${CMAKE_INSTALL_PREFIX}/lib/libprintf.a --no-whole-archive
    ${CMAKE_INSTALL_PREFIX}/lib/libmemory_freelist.a
    ${CMAKE_INSTALL_PREFIX}/lib/libscapula_os.a
    ${CMAKE_INSTALL_PREFIX}/lib/test_case_image.a
    -T ${CONFIGURED_LINKER_SCRIPT}
)

install(TARGETS scapula_loader DESTINATION boot)

# ------------------------------------------------------------------------------
# Scapula loader raw binary (.bin)
# ------------------------------------------------------------------------------

set(SCAPULA_LOADER_BIN ${CMAKE_BINARY_DIR}/scapula_loader.bin)
set(SCAPULA_LOADER_ELF ${CMAKE_BINARY_DIR}/scapula_loader)

add_custom_command(
    COMMAND ${OBJCOPY_BIN} -O binary
        --set-section-flags .bss=alloc,load,contents
        ${SCAPULA_LOADER_ELF} ${SCAPULA_LOADER_BIN}
    OUTPUT ${SCAPULA_LOADER_BIN}
    DEPENDS ${SCAPULA_LOADER_ELF}
    COMMENT "Creating scapula_loader binary: ${SCAPULA_LOADER_BIN}"
)

add_custom_target(scapula_loader_bin ALL DEPENDS scapula_loader ${SCAPULA_LOADER_BIN})

install(FILES ${SCAPULA_LOADER_BIN} DESTINATION boot)
