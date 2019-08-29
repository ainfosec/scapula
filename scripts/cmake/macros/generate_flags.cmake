# Generate Flags
#
# Sets up CMAKE_C_FLAGS and CMAKE_CXX_FLAGS based on the provided flags and
# prefix value. Each time this is executed, CMAKE_XXX_FLAGS are overwritten
# and globally set. The add_subproject function performs this action on your
# behalf, and this function generally should not be used unless you are
# compiling a dependency, in which case, you will need to run this manually.
#
# @param PREFIX (macro arg) Defines which prefix the flags belong too. Valid
#     values are: vmm, userspace, and test
# @param C_FLAGS Additional C flags to add to CMAKE_C_FLAGS
# @param CXX_FLAGS Additional CXX flags to add to CMAKE_CXX_FLAGS
#
function(generate_flags ARCH)
    cmake_parse_arguments(ARG "" "" "" ${ARGN})

    include(${SCAPULA_SOURCE_CMAKE_DIR}/flags/aarch64_flags.cmake)
    include(${SCAPULA_SOURCE_CMAKE_DIR}/flags/aarch32_flags.cmake)

    if(ARCH STREQUAL "aarch64")
        list(APPEND _C_FLAGS ${AARCH64_FLAGS} ${AARCH64_C_FLAGS})
        list(APPEND _CXX_FLAGS ${AARCH64_FLAGS} ${AARCH64_CXX_FLAGS})
    elseif(ARCH STREQUAL "aarch32")
        list(APPEND _C_FLAGS ${AARCH32_FLAGS} ${AARCH32_C_FLAGS})
        list(APPEND _CXX_FLAGS ${AARCH32_FLAGS} ${AARCH32_CXX_FLAGS})
    else()
        message(FATAL_ERROR "Invalid flags '${ARCH}', valid options: 'aarch64' and 'aarch32'")
    endif()

    string(REPLACE ";" " " _C_FLAGS "${_C_FLAGS}")
    string(REPLACE ";" " " _CXX_FLAGS "${_CXX_FLAGS}")

    set(CMAKE_C_FLAGS ${_C_FLAGS} PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS ${_CXX_FLAGS} PARENT_SCOPE)
endfunction(generate_flags)
