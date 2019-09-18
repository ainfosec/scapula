cmake_minimum_required(VERSION 3.6)
project(aarch32_testcases C ASM-ATT)
include(${SCAPULA_SOURCE_CMAKE_DIR}/init_subproject.cmake)

generate_flags(aarch32)

file(GLOB_RECURSE AARCH32_TESTCASE_SRC_FILES ${SCAPULA_HOST_INSTALL_PREFIX}/src/*/aarch32/*.c)

add_executable(aarch32_testcases
    start.s
    ${SCAPULA_HOST_INSTALL_PREFIX}/src/aarch32_main.c
    # ${AARCH32_TESTCASE_SRC_FILES}
)

target_include_directories(aarch32_testcases
    PRIVATE ${CMAKE_INSTALL_PREFIX}/include
)

target_link_libraries(aarch32_testcases
    # ${CMAKE_INSTALL_PREFIX}/lib/libc.a
    # ${CMAKE_INSTALL_PREFIX}/lib/libmemory_freelist.a
    # ${CMAKE_INSTALL_PREFIX}/lib/libprintf.a
    ${CMAKE_INSTALL_PREFIX}/lib/libscapula_os.a
)

install(TARGETS aarch32_testcases DESTINATION bin)
