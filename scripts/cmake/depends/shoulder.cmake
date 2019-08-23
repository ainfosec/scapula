message(STATUS "Including dependency: shoulder")

download_dependency(
    shoulder
    URL          ${SHOULDER_URL}
    URL_MD5      ${SHOULDER_URL_MD5}
)

add_dependency(
    shoulder
    DEPENDS sysreg_spec
    DEPENDS a64_spec
    DEPENDS a32_spec
)
