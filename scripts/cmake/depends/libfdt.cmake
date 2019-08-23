message(STATUS "Including dependency: libfdt")

set(LIBFDT_SOURCE_DIR ${CACHE_DIR}/libfdt)
set(LIBFDT_BUILD_DIR ${DEPENDS_DIR}/libfdt/aarch64-none-elf/build)
set(LIBFDT_MAKEFILE_PATCH ${CMAKE_CURRENT_LIST_DIR}/PatchedLibfdtMakefile)

find_program(LIBFDT_C_COMPILER aarch64-linux-gnu-gcc)
if(NOT LIBFDT_C_COMPILER)
    message(FATAL_ERROR "Unable to find libfdt compiler: ${LIBFDT_C_COMPILER}")
endif()

generate_flags(aarch64)

download_dependency(
    libfdt
    URL          ${DTC_URL}
    URL_MD5      ${DTC_URL_MD5}
)

add_dependency(
    libfdt
    DEPENDS dtc
    CONFIGURE_COMMAND
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBFDT_SOURCE_DIR} ${LIBFDT_BUILD_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIBFDT_MAKEFILE_PATCH} ${LIBFDT_BUILD_DIR}/Makefile
    BUILD_COMMAND
        make -C ${LIBFDT_BUILD_DIR} libfdt
        NO_PYTHON=1
        CC=${LIBFDT_C_COMPILER}
        USER_CFLAGS=${CMAKE_C_FLAGS}
    INSTALL_COMMAND
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBFDT_BUILD_DIR}/libfdt/libfdt.a ${SCAPULA_AARCH64_INSTALL_PREFIX}/lib/libfdt.a
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBFDT_BUILD_DIR}/libfdt/fdt.h ${SCAPULA_AARCH64_INSTALL_PREFIX}/include/fdt.h
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBFDT_BUILD_DIR}/libfdt/libfdt.h ${SCAPULA_AARCH64_INSTALL_PREFIX}/include/libfdt.h
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBFDT_BUILD_DIR}/libfdt/libfdt_env.h ${SCAPULA_AARCH64_INSTALL_PREFIX}/include/libfdt_env.h
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBFDT_BUILD_DIR}/libfdt/libfdt_internal.h ${SCAPULA_AARCH64_INSTALL_PREFIX}/include/libfdt_internal.h
)
