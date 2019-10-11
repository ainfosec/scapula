cmake_minimum_required(VERSION 3.6)
project(scapula_os C ASM-ATT)
include(${SCAPULA_SOURCE_CMAKE_DIR}/init_subproject.cmake)

cmake_policy(SET CMP0076 NEW)

add_library(scapula_os
    assert_fail.c
    banners.c
    panic.c
)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")
    generate_flags(aarch32)
    add_subdirectory(aarch32)
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    add_subdirectory(aarch64)
else()
    message(FATAL_ERROR "Unsupported target: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

target_include_directories(scapula_os
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../include>
        $<INSTALL_INTERFACE:include/scapula_os>  # <prefix>/include/scapula_os
)

target_compile_definitions(scapula_os
    PUBLIC
        CMAKE_CONFIG_SERIAL_BASE=${SERIAL_BASE}
        LOG_LEVEL=${LOG_LEVEL}
)

target_link_libraries(scapula_os
    ${CMAKE_INSTALL_PREFIX}/lib/libc.a
    ${CMAKE_INSTALL_PREFIX}/lib/libmemory_freelist.a
    --whole-archive ${CMAKE_INSTALL_PREFIX}/lib/libprintf.a --no-whole-archive
)

install(TARGETS scapula_os DESTINATION lib)
install(DIRECTORY ../include/ DESTINATION include/scapula_os)
