message(STATUS "Including dependency: libmemory")

set(LIBMEMORY_SOURCE_DIR ${CACHE_DIR}/libmemory)
set(LIBMEMORY_BUILD_DIR ${DEPENDS_DIR}/libmemory/${AARCH64_TARGET_TRIPLE}/build)

generate_flags(aarch64)

# TODO: integrate super-build flags with libc via meson cross file:
# configure_file(${MESON_CROSS_FILE})

ExternalProject_Add(
    libmemory
    DEPENDS             meson
    DOWNLOAD_DIR        ${CACHE_DIR}
    PREFIX              ${DEPENDS_DIR}/libmemory/${AARCH64_TARGET_TRIPLE}
    STAMP_DIR           ${DEPENDS_DIR}/libmemory/${AARCH64_TARGET_TRIPLE}/stamp
    TMP_DIR             ${DEPENDS_DIR}/libmemory/${AARCH64_TARGET_TRIPLE}/tmp
    BINARY_DIR          ${DEPENDS_DIR}/libmemory/${AARCH64_TARGET_TRIPLE}/build
    SOURCE_DIR          ${CACHE_DIR}/libmemory
    GIT_REPOSITORY      ${LIBMEMORY_URL}
    GIT_TAG             1.0.5
    LOG_CONFIGURE 1
    LOG_BUILD 1
    LOG_INSTALL 1
    CONFIGURE_COMMAND
        COMMAND ${MESON_BIN} ${LIBMEMORY_BUILD_DIR} ${LIBMEMORY_SOURCE_DIR} --cross-file=${MESON_CROSS_FILE}
    BUILD_COMMAND
        COMMAND ninja -C ${LIBMEMORY_BUILD_DIR}
    INSTALL_COMMAND
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBMEMORY_BUILD_DIR}/src/libmemory_freelist.a ${SCAPULA_AARCH64_INSTALL_PREFIX}/lib/libmemory_freelist.a
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${LIBMEMORY_SOURCE_DIR}/include ${SCAPULA_AARCH64_INSTALL_PREFIX}/include
)
