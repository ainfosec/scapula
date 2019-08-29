# ------------------------------------------------------------------------------
# System Register Spec
# ------------------------------------------------------------------------------

message(STATUS "Including dependency: ARM XML system register specification")

set(SYSREG_XML_INSTALL_DIR ${SCAPULA_AARCH64_INSTALL_PREFIX}/src/sysreg_spec)
set(SYSREG_SPEC_VERSION ARMv85A-SysReg-00bet9)
set(SYSREG_XML_SUBDIR SysReg_v85A_xml-00bet9)
set(SYSREG_SPEC_URL "https://developer.arm.com/-/media/developer/products/architecture/armv8-a-architecture/${SYSREG_SPEC_VERSION}.tar.gz")

if(SYSREG_XML_PATH STREQUAL NONE)
    set(SYSREG_XML_SRC_DIR ${CACHE_DIR}/sysreg_spec/${SYSREG_XML_SUBDIR})
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


# ------------------------------------------------------------------------------
# A32 ISA Spec
# ------------------------------------------------------------------------------

message(STATUS "Including dependency: ARM XML A32 ISA specification")

set(A32_XML_INSTALL_DIR ${SCAPULA_AARCH64_INSTALL_PREFIX}/src/a32_spec)
set(A32_SPEC_VERSION AArch32_v85A_ISA_xml_00bet9)
set(A32_XML_SUBDIR ISA_v85A_AArch32_xml_00bet9)
set(A32_SPEC_URL "https://developer.arm.com/-/media/developer/products/architecture/armv8-a-architecture/${A32_SPEC_VERSION}.tar.gz")

if(A32_XML_PATH STREQUAL NONE)
    set(A32_XML_SRC_DIR ${CACHE_DIR}/a32_spec/${A32_XML_SUBDIR})
    set(A32_XML_PATH ${A32_XML_SRC_DIR})

    download_dependency(
        a32_spec
        URL          ${A32_SPEC_URL}
        URL_MD5      ${A32_SPEC_MD5}
    )

    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xfz ${A32_XML_SRC_DIR}.tar.gz
        WORKING_DIRECTORY ${CACHE_DIR}/a32_spec
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

# ------------------------------------------------------------------------------
# A64 ISA Spec
# ------------------------------------------------------------------------------

message(STATUS "Including dependency: ARM XML A64 ISA specification")

set(A64_XML_INSTALL_DIR ${SCAPULA_AARCH64_INSTALL_PREFIX}/src/a64_spec)
set(A64_SPEC_VERSION A64_v85A_ISA_xml_00bet9)
set(A64_XML_SUBDIR ISA_v85A_A64_xml_00bet9)
set(A64_SPEC_URL "https://developer.arm.com/-/media/developer/products/architecture/armv8-a-architecture/${A64_SPEC_VERSION}.tar.gz")

if(A64_XML_PATH STREQUAL NONE)
    set(A64_XML_SRC_DIR ${CACHE_DIR}/a64_spec/${A64_XML_SUBDIR})
    set(A64_XML_PATH ${A64_XML_SRC_DIR})

    download_dependency(
        a64_spec
        URL          ${A64_SPEC_URL}
        URL_MD5      ${A64_SPEC_MD5}
    )

    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xfz ${A64_XML_SRC_DIR}.tar.gz
        WORKING_DIRECTORY ${CACHE_DIR}/a64_spec
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
