message(STATUS "Including dependency: ARM xml system register specification")
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
        COMMAND ${CMAKE_COMMAND} -E echo "Installing sysreg spec..."
)

message(STATUS "Including dependency: ARM xml A64 ISA specification")
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
        COMMAND ${CMAKE_COMMAND} -E echo "Installing A64 ISA spec..."
)

message(STATUS "Including dependency: ARM xml A32 ISA specification")
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
        COMMAND ${CMAKE_COMMAND} -E echo "Installing A32 ISA spec..."
)
