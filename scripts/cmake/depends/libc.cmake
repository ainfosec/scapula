message(STATUS "Including dependency: libc")

set(LIBC_SOURCE_DIR ${CACHE_DIR}/libc)
set(LIBC_BUILD_DIR ${DEPENDS_DIR}/libc/${VMM_PREFIX}/build)
set(LIBC_URL "https://github.com/embeddedartistry/libc.git")

set(MESON_BIN ${VMM_PREFIX_PATH}/bin/meson/meson.py)
set(MESON_CROSS_FILE ${SCAPULA_SOURCE_DEPENDS_DIR}/meson-cross-file.txt)

generate_flags(vmm NOWARNINGS)

# TODO: add VMM flags to libc via meson cross file
# configure_file(${MESON_CROSS_FILE})

ExternalProject_Add(
    libc_${VMM_PREFIX}
    DEPENDS             meson_${VMM_PREFIX}
    DOWNLOAD_DIR        ${CACHE_DIR}
    PREFIX              ${DEPENDS_DIR}/${NAME}/${PREFIX}
    STAMP_DIR           ${DEPENDS_DIR}/${NAME}/${PREFIX}/stamp
    TMP_DIR             ${DEPENDS_DIR}/${NAME}/${PREFIX}/tmp
    BINARY_DIR          ${DEPENDS_DIR}/${NAME}/${PREFIX}/build
    SOURCE_DIR          ${CACHE_DIR}/${NAME}
    GIT_REPOSITORY      ${LIBC_URL}
    GIT_TAG             1.0.6
    CONFIGURE_COMMAND
        COMMAND ${MESON_BIN} ${LIBC_BUILD_DIR} ${LIBC_SOURCE_DIR} --cross-file=${MESON_CROSS_FILE}
    BUILD_COMMAND
        COMMAND ninja -C ${LIBC_BUILD_DIR}
    INSTALL_COMMAND
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBC_BUILD_DIR}/src/libc.a ${VMM_PREFIX_PATH}/lib/libc.a
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBC_BUILD_DIR}/src/libprintf.a ${VMM_PREFIX_PATH}/lib/libprintf.a
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${LIBC_SOURCE_DIR}/include ${VMM_PREFIX_PATH}/include
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${LIBC_SOURCE_DIR}/arch/aarch64/include ${VMM_PREFIX_PATH}/include
)
