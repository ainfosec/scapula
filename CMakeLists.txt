cmake_minimum_required(VERSION 3.12)
project(scapula)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(PYTHONPATH ${CMAKE_CURRENT_LIST_DIR}:${VMM_PREFIX_PATH}/lib/python3/dist-packages)

set(SCAPULA_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/scapula)
set(SCAPULA_STAMP_DIR ${SCAPULA_BUILD_DIR}/stamp)
file(MAKE_DIRECTORY ${SCAPULA_STAMP_DIR})

# ------------------------------------------------------------------------------
# Generate Scaupla Tests
# ------------------------------------------------------------------------------

file(GLOB_RECURSE SCAPULA_SRC_FILES "scapula/*.py")

set(SCAPULA_OUTPUT_STAMP ${SCAPULA_STAMP_DIR}/scapula_output.stamp)

add_custom_command(
    COMMAND PYTHONPATH=${PYTHONPATH} ${Python3_EXECUTABLE} -m scapula
    COMMAND cmake -E touch ${SCAPULA_OUTPUT_STAMP}
    OUTPUT ${SCAPULA_OUTPUT_STAMP}
    DEPENDS ${SCAPULA_SRC_FILES}
    COMMENT "Generating scapula outputs"
    VERBATIM
)

add_custom_target(generate ALL DEPENDS ${SCAPULA_OUTPUT_STAMP})

# ------------------------------------------------------------------------------
# Install
# ------------------------------------------------------------------------------

install(DIRECTORY scapula DESTINATION lib/python3/dist-packages)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/ DESTINATION include/scapula)

