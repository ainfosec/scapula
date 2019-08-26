set(SYSREG_XML_INSTALL_DIR ${SCAPULA_AARCH64_INSTALL_PREFIX}/src/sysreg_spec)
set(A32_XML_INSTALL_DIR ${SCAPULA_AARCH64_INSTALL_PREFIX}/src/a32_spec)
set(A64_XML_INSTALL_DIR ${SCAPULA_AARCH64_INSTALL_PREFIX}/src/a64_spec)

# If build config doesn't specify a spec, download the latest and update the
# build configuration to use the downloaded version
message(STATUS "Including dependency: ARM xml system register specification")
if(SYSREG_XML_PATH STREQUAL NONE)
    set(SYSREG_XML_SRC_DIR ${CACHE_DIR}/sysreg_spec/SysReg_xml_v85A-2019-06)
    set(SYSREG_XML_PATH ${SYSREG_XML_SRC_DIR})

    download_dependency(
        sysreg_spec
        URL          ${SYSREG_SPEC_URL}
        URL_MD5      ${SYSREG_SPEC_MD5}
    )

    add_dependency(
        sysreg_spec
        CONFIGURE_COMMAND
            COMMAND ${CMAKE_COMMAND} -E echo "Configuring sysreg spec..."
        BUILD_COMMAND
            COMMAND ${CMAKE_COMMAND} -E echo "Building sysreg spec..."
        INSTALL_COMMAND
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${SYSREG_XML_SRC_DIR} ${SYSREG_XML_INSTALL_DIR}
)
else()
    message(STATUS "    using sysreg XML spec: ${SYSREG_XML_PATH}")
    set(SYSREG_XML_SRC_DIR ${SYSREG_XML_PATH})

    add_custom_target(
        sysreg_spec
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SYSREG_XML_SRC_DIR} ${SYSREG_XML_INSTALL_DIR}
        COMMENT "Installing sysreg XML spec..."
    )
endif()



# If build config doesn't specify a spec, download the latest and update the
# build configuration to use the downloaded version
message(STATUS "Including dependency: ARM xml A64 ISA specification")
if(A64_XML_PATH STREQUAL NONE)
    set(A64_XML_SRC_DIR ${CACHE_DIR}/a64_spec/ISA_A64_xml_v85A-2019-06)
    set(A64_XML_PATH ${A64_XML_SRC_DIR})

    download_dependency(
        a64_spec
        URL          ${A64_SPEC_URL}
        URL_MD5      ${A64_SPEC_MD5}
    )

    add_dependency(
        a64_spec
        CONFIGURE_COMMAND
            COMMAND ${CMAKE_COMMAND} -E echo "Configuring A64 ISA spec..."
        BUILD_COMMAND
            COMMAND ${CMAKE_COMMAND} -E echo "Building A64 ISA spec..."
        INSTALL_COMMAND
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${A64_XML_SRC_DIR} ${A64_XML_INSTALL_DIR}
    )
else()
    message(STATUS "    using A64 ISA XML spec: ${A64_XML_PATH}")
    set(A64_XML_SRC_DIR ${A64_XML_PATH})

    add_custom_target(
        a64_spec
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${A64_XML_SRC_DIR} ${A64_XML_INSTALL_DIR}
        COMMENT "Installing A64 ISA XML spec..."
    )
endif()

# If build config doesn't specify a spec, download the latest and update the
# build configuration to use the downloaded version
message(STATUS "Including dependency: ARM xml A32 ISA specification")
if(A32_XML_PATH STREQUAL NONE)
    set(A32_XML_SRC_DIR ${CACHE_DIR}/a32_spec/ISA_AArch32_xml_v85A-2019-06)
    set(A32_XML_PATH ${A32_XML_SRC_DIR})

    download_dependency(
        a32_spec
        URL          ${A32_SPEC_URL}
        URL_MD5      ${A32_SPEC_MD5}
    )

    add_dependency(
        a32_spec
        CONFIGURE_COMMAND
            COMMAND ${CMAKE_COMMAND} -E echo "Configuring A32 ISA spec..."
        BUILD_COMMAND
            COMMAND ${CMAKE_COMMAND} -E echo "Building A32 ISA spec..."
        INSTALL_COMMAND
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${A32_XML_SRC_DIR} ${A32_XML_INSTALL_DIR}
    )

else()
    message(STATUS "    using A32 ISA XML spec: ${A32_XML_PATH}")
    set(A32_XML_SRC_DIR ${A32_XML_PATH})

    add_custom_target(
        A32_spec
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${A32_XML_SRC_DIR} ${A32_XML_INSTALL_DIR}
        COMMENT "Installing A32 ISA XML spec..."
    )
endif()
