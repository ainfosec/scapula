if(ENABLE_BUILD_SCAPULA AND NOT WIN32)
    message(STATUS "Including dependency: dtc")

    set(DTC_SOURCE_DIR ${CACHE_DIR}/dtc)
    set(DTC_BUILD_DIR ${DEPENDS_DIR}/dtc/${VMM_PREFIX}/build)

    download_dependency(
        dtc
        URL          ${DTC_URL}
        URL_MD5      ${DTC_URL_MD5}
    )

    add_dependency(
        dtc vmm
        CONFIGURE_COMMAND   ${CMAKE_COMMAND} -E copy_directory ${DTC_SOURCE_DIR} ${DTC_BUILD_DIR}
        BUILD_COMMAND       make -C ${DTC_BUILD_DIR} NO_PYTHON=1
        INSTALL_COMMAND     ${CMAKE_COMMAND} -E copy ${DTC_BUILD_DIR}/dtc ${VMM_PREFIX_PATH}/bin/dtc
    )
endif()
