message(STATUS "Including dependency: libfdt")

set(LIBFDT_SOURCE_DIR ${CACHE_DIR}/libfdt)
set(LIBFDT_BUILD_DIR ${DEPENDS_DIR}/libfdt/${VMM_PREFIX}/build)
set(LIBFDT_C_COMPILER aarch64-linux-gnu-gcc)
set(LIBFDT_MAKEFILE_PATCH ${CMAKE_CURRENT_LIST_DIR}/PatchedLibfdtMakefile)

generate_flags(vmm NOWARNINGS)

download_dependency(
    libfdt
    URL          ${DTC_URL}
    URL_MD5      ${DTC_URL_MD5}
)

add_dependency(
    libfdt vmm
    DEPENDS dtc_${VMM_PREFIX}
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
            ${LIBFDT_BUILD_DIR}/libfdt/libfdt.a ${VMM_PREFIX_PATH}/lib/libfdt.a
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBFDT_BUILD_DIR}/libfdt/fdt.h ${VMM_PREFIX_PATH}/include/fdt.h
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBFDT_BUILD_DIR}/libfdt/libfdt.h ${VMM_PREFIX_PATH}/include/libfdt.h
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBFDT_BUILD_DIR}/libfdt/libfdt_env.h ${VMM_PREFIX_PATH}/include/libfdt_env.h
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBFDT_BUILD_DIR}/libfdt/libfdt_internal.h ${VMM_PREFIX_PATH}/include/libfdt_internal.h
)
