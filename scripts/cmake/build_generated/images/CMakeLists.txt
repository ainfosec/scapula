cmake_minimum_required(VERSION 3.6)
project(images C CXX)
include(${SCAPULA_SOURCE_CMAKE_DIR}/init_subproject.cmake)

# ------------------------------------------------------------------------------
# Test case flattened image tree (.fit)
# Contains all sub-images to be used as the payload for Scapula OS 
# ------------------------------------------------------------------------------

set(TESTCASE_ITS ${SCAPULA_SOURCE_ROOT_DIR}/scripts/device_tree/testcases.its)
set(TESTCASE_ITB ${CMAKE_CURRENT_BINARY_DIR}/scapula_payload.itb)

add_custom_command(
    COMMAND ${DTC_BIN} --quiet --align 1024 -I dts -O dtb
        -i ${SCAPULA_AARCH64_INSTALL_PREFIX}/bin
        -i ${SCAPULA_AARCH32_INSTALL_PREFIX}/bin
        -o ${TESTCASE_ITB} ${TESTCASE_ITS}
    OUTPUT ${TESTCASE_ITB}
    DEPENDS ${TESTCASE_ITS}
    DEPENDS ${CMAKE_INSTALL_PREFIX}/bin/aarch64_testcases
    DEPENDS ${CMAKE_INSTALL_PREFIX}/lib/libscapula_os.a
    COMMENT "Creating testcase .fit image using: ${TESTCASE_ITS}"
)

add_custom_target(testcase_itb_target ALL DEPENDS ${TESTCASE_ITB})

# ------------------------------------------------------------------------------
# Test case image archive (.a)
# Constains the above .fit image, linkable to an aarch64 ELF executable
# ------------------------------------------------------------------------------

set(PAYLOAD_IMAGE_OBJ ${CMAKE_CURRENT_BINARY_DIR}/test_case_image.o)
set(PAYLOAD_IMAGE_AR ${CMAKE_CURRENT_BINARY_DIR}/test_case_image.a)

add_custom_command(
    COMMAND ${OBJCOPY_BIN}
        -I binary
        -O elf64-littleaarch64
        -B aarch64
        ${TESTCASE_ITB}
        ${PAYLOAD_IMAGE_OBJ}
        --add-symbol=test_case_image=.data:0
    OUTPUT ${PAYLOAD_IMAGE_OBJ}
    DEPENDS ${TESTCASE_ITB}
    COMMENT "Creating payload image object file: ${PAYLOAD_IMAGE_OBJ}"
)

add_custom_target(test_case_image_obj ALL DEPENDS ${PAYLOAD_IMAGE_OBJ})

add_custom_command(
    COMMAND ${AR_BIN} rcs ${PAYLOAD_IMAGE_AR} ${PAYLOAD_IMAGE_OBJ}
    OUTPUT ${PAYLOAD_IMAGE_AR}
    DEPENDS ${TESTCASE_ITB}
    COMMENT "Creating payload image archive: ${PAYLOAD_IMAGE_AR}"
)

add_custom_target(test_case_image_ar ALL DEPENDS test_case_image_obj ${PAYLOAD_IMAGE_AR})

install(FILES ${PAYLOAD_IMAGE_AR} DESTINATION lib)
