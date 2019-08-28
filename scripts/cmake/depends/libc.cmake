# ------------------------------------------------------------------------------
# Libc
# ------------------------------------------------------------------------------

message(STATUS "Including dependency: libc")

set(LIBC_URL "https://github.com/jaredwright/libc/archive/1.0.6.zip")
set(LIBC_URL_MD5 "d766ef8d9031122371183c9f799ee981")
set(LIBC_SOURCE_DIR ${CACHE_DIR}/libc)
set(LIBC_BUILD_DIR ${DEPENDS_DIR}/libc/${AARCH64_TARGET_TRIPLE}/build)

generate_flags(aarch64)

# TODO: integrate super-build flags with libc via meson cross file:
# configure_file(${MESON_CROSS_FILE})

download_dependency(
    libc
    URL          ${LIBC_URL}
    URL_MD5      ${LIBC_URL_MD5}
)

add_dependency(
    libc
    DEPENDS meson
    CONFIGURE_COMMAND
        COMMAND ${MESON_BIN} ${LIBC_BUILD_DIR} ${LIBC_SOURCE_DIR}
            --cross-file=${MESON_CROSS_FILE}
    BUILD_COMMAND
        COMMAND ninja -C ${LIBC_BUILD_DIR}
    INSTALL_COMMAND
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBC_BUILD_DIR}/src/libc.a
            ${SCAPULA_AARCH64_INSTALL_PREFIX}/lib/libc.a
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIBC_BUILD_DIR}/src/libprintf.a
            ${SCAPULA_AARCH64_INSTALL_PREFIX}/lib/libprintf.a
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${LIBC_SOURCE_DIR}/include
            ${SCAPULA_AARCH64_INSTALL_PREFIX}/include
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${LIBC_SOURCE_DIR}/arch/aarch64/include
            ${SCAPULA_AARCH64_INSTALL_PREFIX}/include
)

# ------------------------------------------------------------------------------
# Libc Submodules
# ------------------------------------------------------------------------------

message(STATUS "Including dependency: gdtoa")

set(GDTOA_URL "https://github.com/JaredWright/gdtoa/archive/0f19a34ab19ddf9d1bc5ab4f5114f3abcc117dc6.zip")
set(GDTOA_URL_MD5 "4a5ba50716fd09ee65d8f681868b412f")

download_dependency(
    gdtoa
    URL          ${GDTOA_URL}
    URL_MD5      ${GDTOA_URL_MD5}
    DESTINATION  ${CACHE_DIR}/libc/src/gdtoa
)

message(STATUS "Including dependency: printf")

set(PRINTF_URL "https://github.com/jaredwright/printf/archive/v4.0.0.zip")
set(PRINTF_URL_MD5 "cd2aa8ca491feef17be5bd6bfb87e6c6")

download_dependency(
    printf
    URL          ${PRINTF_URL}
    URL_MD5      ${PRINTF_URL_MD5}
    DESTINATION  ${CACHE_DIR}/libc/printf
)

message(STATUS "Including dependency: openlibm")

set(OPENLIBM_URL "https://github.com/jaredwright/openlibm/archive/v0.6.0.zip")
set(OPENLIBM_URL_MD5 "56f87932d361a54c8807c69976652a9e")

download_dependency(
    openlibm
    URL          ${OPENLIBM_URL}
    URL_MD5      ${OPENLIBM_URL_MD5}
    DESTINATION  ${CACHE_DIR}/libc/openlibm
)

message(STATUS "Including dependency: meson-buildsystem")

set(MESON_BUILDSYSTEM_URL "https://github.com/jaredwright/meson-buildsystem/archive/master.zip")

download_dependency(
    meson-buildsystem
    URL          ${MESON_BUILDSYSTEM_URL}
    DESTINATION  ${CACHE_DIR}/libc/build
)
