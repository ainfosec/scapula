message(STATUS "Including dependency: libc")

set(LIBC_SOURCE_DIR ${CACHE_DIR}/libc)
set(LIBC_BUILD_DIR ${DEPENDS_DIR}/libc/${AARCH64_TARGET_TRIPLE}/build)

generate_flags(aarch64)

# TODO: integrate super-build flags with libc via meson cross file:
# configure_file(${MESON_CROSS_FILE})

ExternalProject_Add(
    libc
    DEPENDS             meson
    DOWNLOAD_DIR        ${CACHE_DIR}
    PREFIX              ${DEPENDS_DIR}/libc/${AARCH64_TARGET_TRIPLE}
    STAMP_DIR           ${DEPENDS_DIR}/libc/${AARCH64_TARGET_TRIPLE}/stamp
    TMP_DIR             ${DEPENDS_DIR}/libc/${AARCH64_TARGET_TRIPLE}/tmp
    BINARY_DIR          ${DEPENDS_DIR}/libc/${AARCH64_TARGET_TRIPLE}/build
    SOURCE_DIR          ${CACHE_DIR}/libc
    GIT_REPOSITORY      ${LIBC_URL}
    GIT_TAG             1.0.6
    LOG_CONFIGURE 1
    LOG_BUILD 1
    LOG_INSTALL 1
    CONFIGURE_COMMAND
        COMMAND ${MESON_BIN} ${LIBC_BUILD_DIR} ${LIBC_SOURCE_DIR} --cross-file=${MESON_CROSS_FILE}
    BUILD_COMMAND
        COMMAND ninja -C ${LIBC_BUILD_DIR}
    INSTALL_COMMAND
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBC_BUILD_DIR}/src/libc.a ${SCAPULA_AARCH64_INSTALL_PREFIX}/lib/libc.a
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBC_BUILD_DIR}/src/libprintf.a ${SCAPULA_AARCH64_INSTALL_PREFIX}/lib/libprintf.a
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${LIBC_SOURCE_DIR}/include ${SCAPULA_AARCH64_INSTALL_PREFIX}/include
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${LIBC_SOURCE_DIR}/arch/aarch64/include ${SCAPULA_AARCH64_INSTALL_PREFIX}/include
)
