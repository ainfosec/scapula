message(STATUS "Including dependency: meson")

set(MESON_SOURCE_DIR ${CACHE_DIR}/meson)
set(MESON_INSTALL_DIR ${VMM_PREFIX_PATH}/bin/meson)
set(MESON_BIN ${MESON_INSTALL_DIR}/meson/meson.py)

set(MESON_URL "https://github.com/mesonbuild/meson/releases/download/0.51.1/meson-0.51.1.tar.gz")
set(MESON_URL_MD5 "48787e391ec5c052799a3dd491f73909")

download_dependency(
    meson
    URL          ${MESON_URL}
    URL_MD5      ${MESON_URL_MD5}
)

add_dependency(
    meson vmm
    CONFIGURE_COMMAND
        COMMAND ${CMAKE_COMMAND} -E echo "Configuring meson..."
    BUILD_COMMAND
        COMMAND ${CMAKE_COMMAND} -E echo "Building meson..."
    INSTALL_COMMAND
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${MESON_SOURCE_DIR} ${MESON_INSTALL_DIR}
)
