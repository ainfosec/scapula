# Add Dependency
#
# Uses ExternalProject_Add to add the dependency to the build. All of the
# optional arguments are passed directly to ExternalProject_Add, so most of
# ExternalProject_Add's options are supported by this function.
#
# @param NAME the name of the dependency
#
function(add_dependency NAME)
    set(oneVal INSTALL_PREFIX)
    cmake_parse_arguments(ARG "" "${oneVal}" "${multiVal}" ${ARGN})

    if(NOT ARG_INSTALL_PREFIX)
        set(ARG_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
    endif()

    if(EXISTS "${CACHE_DIR}/${NAME}/CMakeLists.txt")
        list(APPEND ARGN
            CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${ARG_INSTALL_PREFIX}
            CMAKE_ARGS -DCMAKE_INSTALL_MESSAGE=LAZY
            CMAKE_ARGS -DCMAKE_TARGET_MESSAGES=OFF
            CMAKE_ARGS -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}
            CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        )
    else()
        list(APPEND ARGN
            LOG_CONFIGURE 1
            LOG_BUILD 1
            LOG_INSTALL 1
        )
    endif()

    ExternalProject_Add(
        ${NAME}
        ${ARGN}
        PREFIX              ${DEPENDS_DIR}/${NAME}/
        STAMP_DIR           ${DEPENDS_DIR}/${NAME}/stamp
        TMP_DIR             ${DEPENDS_DIR}/${NAME}/tmp
        BINARY_DIR          ${DEPENDS_DIR}/${NAME}/build
        SOURCE_DIR          ${CACHE_DIR}/${NAME}
    )
endfunction(add_dependency)
