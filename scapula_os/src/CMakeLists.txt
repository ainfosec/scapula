cmake_minimum_required(VERSION 3.6)
project(scapula_os C ASM-ATT)
include(${SCAPULA_SOURCE_CMAKE_DIR}/init_subproject.cmake)

cmake_policy(SET CMP0076 NEW)

add_library(scapula_os
    panic.c
    banners.c
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
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../include
)

target_compile_definitions(scapula_os
    PRIVATE CMAKE_CONFIG_SERIAL_BASE=${SERIAL_BASE}
)

install(TARGETS scapula_os DESTINATION lib)
install(DIRECTORY ../include/ DESTINATION include/scapula_os)
