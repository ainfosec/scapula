message(STATUS "Including dependency: libmemory")

set(LIBMEMORY_SOURCE_DIR ${CACHE_DIR}/libmemory)
set(LIBMEMORY_BUILD_DIR ${DEPENDS_DIR}/libmemory/${VMM_PREFIX}/build)
set(LIBMEMORY_URL "https://github.com/embeddedartistry/libmemory.git")

set(MESON_BIN ${VMM_PREFIX_PATH}/bin/meson/meson.py)
set(MESON_CROSS_FILE ${SCAPULA_SOURCE_DEPENDS_DIR}/meson-cross-file.txt)

generate_flags(vmm NOWARNINGS)

# TODO: add VMM flags to libmemory via meson cross file
# configure_file(${MESON_CROSS_FILE})

ExternalProject_Add(
    libmemory_${VMM_PREFIX}
    DEPENDS             meson_${VMM_PREFIX}
    DOWNLOAD_DIR        ${CACHE_DIR}
    PREFIX              ${DEPENDS_DIR}/${NAME}/${PREFIX}
    STAMP_DIR           ${DEPENDS_DIR}/${NAME}/${PREFIX}/stamp
    TMP_DIR             ${DEPENDS_DIR}/${NAME}/${PREFIX}/tmp
    BINARY_DIR          ${DEPENDS_DIR}/${NAME}/${PREFIX}/build
    SOURCE_DIR          ${CACHE_DIR}/${NAME}
    GIT_REPOSITORY      ${LIBMEMORY_URL}
    GIT_TAG             1.0.5
    CONFIGURE_COMMAND
        COMMAND ${MESON_BIN} ${LIBMEMORY_BUILD_DIR} ${LIBMEMORY_SOURCE_DIR} --cross-file=${MESON_CROSS_FILE}
    BUILD_COMMAND
        COMMAND ninja -C ${LIBMEMORY_BUILD_DIR}
    INSTALL_COMMAND
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBMEMORY_BUILD_DIR}/src/libmemory_freelist.a ${VMM_PREFIX_PATH}/lib/libmemory_freelist.a
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${LIBMEMORY_SOURCE_DIR}/include ${VMM_PREFIX_PATH}/include
)
