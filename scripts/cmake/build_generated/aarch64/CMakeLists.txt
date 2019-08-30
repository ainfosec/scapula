cmake_minimum_required(VERSION 3.6)
project(aarch64_testcases C)
include(${SCAPULA_SOURCE_CMAKE_DIR}/init_subproject.cmake)

file(GLOB_RECURSE AARCH64_TESTCASE_SRC_FILES ${CMAKE_INSTALL_PREFIX}/src/*/aarch64/*.c)

add_library(aarch64_testcases ${AARCH64_TESTCASE_SRC_FILES})

install(TARGETS aarch64_testcases DESTINATION lib)
