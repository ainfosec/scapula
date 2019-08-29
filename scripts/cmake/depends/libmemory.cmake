# ------------------------------------------------------------------------------
# Libmemory
# ------------------------------------------------------------------------------

message(STATUS "Including dependency: libmemory")

set(LIBMEMORY_URL "https://github.com/jaredwright/libmemory/archive/1.0.5.zip")
set(LIBMEMORY_URL_MD5 "36bddd9bd4f402ed7779cb084c7e2b09")
set(LIBMEMORY_SOURCE_DIR ${CACHE_DIR}/libmemory)
set(LIBMEMORY_BUILD_DIR ${DEPENDS_DIR}/libmemory/${AARCH64_TARGET_TRIPLE}/build)

generate_flags(aarch64)

# TODO: integrate super-build flags with libc via meson cross file:
# configure_file(${MESON_CROSS_FILE})

download_dependency(
    libmemory
    URL          ${LIBMEMORY_URL}
    URL_MD5      ${LIBMEMORY_URL_MD5}
)

add_dependency(
    libmemory
    DEPENDS meson
    CONFIGURE_COMMAND
        COMMAND ${MESON_BIN} ${LIBMEMORY_BUILD_DIR} ${LIBMEMORY_SOURCE_DIR}
            --cross-file=${MESON_CROSS_FILE}
    BUILD_COMMAND
        COMMAND ninja -C ${LIBMEMORY_BUILD_DIR}
    INSTALL_COMMAND
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBMEMORY_BUILD_DIR}/src/libmemory_freelist.a
            ${SCAPULA_AARCH64_INSTALL_PREFIX}/lib/libmemory_freelist.a
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${LIBMEMORY_SOURCE_DIR}/include
            ${SCAPULA_AARCH64_INSTALL_PREFIX}/include
)

# ------------------------------------------------------------------------------
# Libmemory Submodules
# ------------------------------------------------------------------------------

message(STATUS "Including dependency: meson-buildsystem")

set(MESON_BUILDSYSTEM_URL "https://github.com/jaredwright/meson-buildsystem/archive/master.zip")

download_dependency(
    meson-buildsystem
    URL          ${MESON_BUILDSYSTEM_URL}
    DESTINATION  ${CACHE_DIR}/libmemory/build
)
