cmake_minimum_required(VERSION 3.12)
project(scapula NONE)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(PYTHONPATH ${SCAPULA_SOURCE_ROOT_DIR}:${SCAPULA_HOST_INSTALL_PREFIX}/lib/python3/dist-packages)

set(SCAPULA_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(SCAPULA_STAMP_DIR ${SCAPULA_BUILD_DIR}/stamp)
file(MAKE_DIRECTORY ${SCAPULA_STAMP_DIR})

# ------------------------------------------------------------------------------
# Generate Scaupla Tests
# ------------------------------------------------------------------------------

file(GLOB_RECURSE SCAPULA_SRC_FILES "*.py")

set(SCAPULA_OUTPUT_STAMP ${SCAPULA_STAMP_DIR}/scapula_output.stamp)

add_custom_command(
    COMMAND PYTHONPATH=${PYTHONPATH} ${Python3_EXECUTABLE} -m scapula
        -i ${SYSREG_XML_PATH}
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

set(CMAKE_INSTALL_MESSAGE NEVER)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR} DESTINATION lib/python3/dist-packages)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/ DESTINATION ${SCAPULA_HOST_INSTALL_PREFIX}/src)
