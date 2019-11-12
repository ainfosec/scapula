message(STATUS "Including dependency: shoulder")

set(SHOULDER_URL "https://github.com/ainfosec/shoulder/archive/master.zip")
set(SHOULDER_SOURCE_DIR ${CACHE_DIR}/shoulder)
set(SHOULDER_BUILD_DIR ${DEPENDS_DIR}/shoulder/${AARCH64_TARGET_TRIPLE}/build)
set(SHOULDER_INSTALL_DIR ${SCAPULA_HOST_INSTALL_PREFIX}/bin/shoulder)

set(SHOULDER_BIN ${SHOULDER_INSTALL_DIR}/shoulder.py
    CACHE INTERNAL
    "Path to installed Shoulder python script"
)

download_dependency(
    shoulder
    URL          ${SHOULDER_URL}
    URL_MD5      ${SHOULDER_URL_MD5}
)

add_dependency(
    shoulder
    CONFIGURE_COMMAND
        COMMAND ${CMAKE_COMMAND} -E
            copy_directory ${SHOULDER_SOURCE_DIR} ${SHOULDER_BUILD_DIR}
    BUILD_COMMAND
        COMMAND ${SHOULDER_BUILD_DIR}/shoulder.py
            -i ${SYSREG_XML_PATH}
            -o ${SCAPULA_AARCH64_INSTALL_PREFIX}/include/shoulder
    INSTALL_COMMAND
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SHOULDER_BUILD_DIR}/shoulder
            ${SCAPULA_HOST_INSTALL_PREFIX}/lib/python3/dist-packages/shoulder
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SHOULDER_BUILD_DIR}/scripts
            ${SCAPULA_HOST_INSTALL_PREFIX}/lib/python3/dist-packages/scripts
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SHOULDER_BUILD_DIR}/include
            ${SCAPULA_HOST_INSTALL_PREFIX}/lib/python3/dist-packages/include
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SHOULDER_SOURCE_DIR}/shoulder
            ${SCAPULA_HOST_INSTALL_PREFIX}/bin/shoulder
)
