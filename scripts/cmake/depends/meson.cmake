message(STATUS "Including dependency: meson")

set(MESON_SOURCE_DIR ${CACHE_DIR}/meson)
set(MESON_INSTALL_DIR ${SCAPULA_AARCH64_INSTALL_PREFIX}/bin/meson)

set(MESON_BIN ${MESON_INSTALL_DIR}/meson.py
    CACHE INTERNAL
    "Path to installed Meson build system binary"
)

set(MESON_CROSS_FILE ${SCAPULA_SOURCE_DEPENDS_DIR}/meson-cross-file.txt
    CACHE INTERNAL
    "Path to Meson cross file for building Meson based dependencies"
)

download_dependency(
    meson
    URL          ${MESON_URL}
    URL_MD5      ${MESON_URL_MD5}
)

add_dependency(
    meson
    LOG_CONFIGURE 1
    LOG_BUILD 1
    LOG_INSTALL 1
    CONFIGURE_COMMAND
        COMMAND ${CMAKE_COMMAND} -E echo "Configuring meson..."
    BUILD_COMMAND
        COMMAND ${CMAKE_COMMAND} -E echo "Building meson..."
    INSTALL_COMMAND
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${MESON_SOURCE_DIR} ${MESON_INSTALL_DIR}
)
