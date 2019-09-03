cmake_minimum_required(VERSION 3.6)
project(aarch32_testcases C)
include(${SCAPULA_SOURCE_CMAKE_DIR}/init_subproject.cmake)

generate_flags(aarch32)

file(GLOB_RECURSE AARCH32_TESTCASE_SRC_FILES ${CMAKE_INSTALL_PREFIX}/src/*/aarch32/*.c)

add_library(aarch32_testcases ${AARCH32_TESTCASE_SRC_FILES})

install(TARGETS aarch32_testcases DESTINATION lib)
