message(STATUS "Including dependency: dtc")

set(DTC_URL "https://github.com/jaredwright/dtc/archive/v1.4.6.zip")
set(DTC_URL_MD5 "540fb180485cd98b73800d39f2993a29")
set(DTC_SOURCE_DIR ${CACHE_DIR}/dtc)
set(DTC_BUILD_DIR ${DEPENDS_DIR}/dtc/${AARCH64_TARGET_TRIPLE}/build)

download_dependency(
    dtc
    URL          ${DTC_URL}
    URL_MD5      ${DTC_URL_MD5}
)

add_dependency(
    dtc
    CONFIGURE_COMMAND   ${CMAKE_COMMAND} -E copy_directory ${DTC_SOURCE_DIR} ${DTC_BUILD_DIR}
    BUILD_COMMAND       make -C ${DTC_BUILD_DIR} NO_PYTHON=1
    INSTALL_COMMAND     ${CMAKE_COMMAND} -E copy ${DTC_BUILD_DIR}/dtc ${SCAPULA_AARCH64_INSTALL_PREFIX}/bin/dtc
)
