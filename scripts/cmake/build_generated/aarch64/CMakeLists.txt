cmake_minimum_required(VERSION 3.6)
project(aarch64_testcases C ASM-ATT)

include(${SCAPULA_SOURCE_CMAKE_DIR}/init_subproject.cmake)

file(GLOB_RECURSE AARCH64_TESTCASE_SRC_FILES ${SCAPULA_HOST_INSTALL_PREFIX}/src/*/aarch64/*.c)

add_executable(aarch64_testcases
    start.s
    ${SCAPULA_HOST_INSTALL_PREFIX}/src/aarch64_main.c
    ${AARCH64_TESTCASE_SRC_FILES}
)

target_include_directories(aarch64_testcases
    PRIVATE ${CMAKE_INSTALL_PREFIX}/include
)

target_compile_definitions(aarch64_testcases
    PUBLIC
        CMAKE_CONFIG_SERIAL_BASE=${SERIAL_BASE}
        LOG_LEVEL=${LOG_LEVEL}
)

target_link_libraries(aarch64_testcases
    ${CMAKE_INSTALL_PREFIX}/lib/libc.a
    --whole-archive ${CMAKE_INSTALL_PREFIX}/lib/libmemory_freelist.a --no-whole-archive
    --whole-archive ${CMAKE_INSTALL_PREFIX}/lib/libprintf.a --no-whole-archive
    --whole-archive ${CMAKE_INSTALL_PREFIX}/lib/libscapula_os.a --no-whole-archive
)

install(TARGETS aarch64_testcases DESTINATION bin)
