# Download Dependency
#
# Downloads a dependency from a URL. Dependencies can either be
# a tarball or a zip file. These downloaded files are placeed in the CACHE_DIR,
# or DESTINATION if specified.
#
# @param NAME the name of the dependency
# @param URL The URL for the dependency
# @param URL_MD5 Optional MD5 of the file being downloaded
# @param DESTINATION Destination path to download the file to
#
function(download_dependency NAME)
    set(oneVal URL URL_MD5 DESTINATION)
    cmake_parse_arguments(ARG "" "${oneVal}" "" ${ARGN})

    if(NOT ARG_URL)
        message(FATAL_ERROR "${Red}download_dependency: no URL specified${ColorReset}")
    endif()

    if(NOT ARG_DESTINATION)
        set(ARG_DESTINATION ${CACHE_DIR}/${NAME})
    endif()

    # If the dependency is already downloaded, return
    # set(SRC ${ARG_DESTINATION}/${NAME})
    set(SRC ${ARG_DESTINATION})
    file(GLOB SRC_CONTENTS "${SRC}/*")
    list(LENGTH SRC_CONTENTS SRC_CONTENTS_LEN)
    if(SRC_CONTENTS_LEN GREATER 0)
        message(STATUS "    ${Green}found${ColorReset}: ${SRC}")
        return()
    endif()

    # Get the file extention type
    string(REGEX REPLACE "\\.[^.]*$" "" FILENAME ${ARG_URL})
    string(REPLACE "${FILENAME}" "" EXT ${ARG_URL})
    get_filename_component(LONG_EXT ${ARG_URL} EXT)
    if(NOT LONG_EXT MATCHES "(\\.|=)(7z|tar\\.bz2|tar\\.gz|tar\\.xz|tbz2|tgz|txz|zip)$")
        message(FATAL_ERROR "Unsupported file format: ${ARG_URL}")
    endif()

    if(LONG_EXT MATCHES ".tar.gz$")
        set(EXT ".tar.gz")
    endif()

    if(LONG_EXT MATCHES ".tar.xz$")
        set(EXT ".tar.xz")
    endif()

    if(LONG_EXT MATCHES ".tar.bz2$")
        set(EXT ".tar.bz2")
    endif()

    set(TMP ${ARG_DESTINATION}_tmp)
    set(TAR ${ARG_DESTINATION}${EXT})

    # Attempt download up to 5 times
    foreach(ATTEMPT RANGE 1 5 1)
        if(EXISTS "${TAR}")
            # If an MD5 has specified, check the value against the download
            if(ARG_URL_MD5)
                message(STATUS "    checking hash: ${ARG_URL_MD5}")
                file(MD5 ${TAR} MD5)
                if(NOT "${MD5}" STREQUAL "${ARG_URL_MD5}")
                    message(STATUS "    ${Red}md5 mismatch: expecting ${ARG_URL_MD5}, got ${MD5}${ColorReset}")
                    set_property(GLOBAL PROPERTY "FORCE_REBUILD" "ON")
                    file(REMOVE_RECURSE ${SRC})
                    file(REMOVE_RECURSE ${TMP})
                    file(REMOVE_RECURSE ${TAR})
                    message(STATUS "    checking hash: ${Yellow}complete, redownload required${ColorReset}")
                else()
                    message(STATUS "    checking hash: ${Green}complete${ColorReset}")
                    break()
                endif()
            else()
                break()
            endif()
        endif()

        if(ATTEMPT GREATER 1)
            message(STATUS "    attempt: ${ATTEMPT}")
        endif()

        message(STATUS "    download file: ${ARG_URL} -> ${TAR}")
        file(DOWNLOAD ${ARG_URL} ${TAR} STATUS DOWNLOAD_STATUS)
        if(NOT DOWNLOAD_STATUS MATCHES "0;")
            message(STATUS "    ${Red}failed to download ${ARG_URL}${ColorReset}")
            file(REMOVE_RECURSE ${TAR})
            continue()
        else()
            message(STATUS "    download file: ${Green}complete${ColorReset}")
            break()
        endif()
    endforeach()

    if(NOT EXISTS ${TAR})
        message(FATAL_ERROR "Failed to download ${ARG_URL}")
    endif()

    # Unpack the download
    if(NOT EXISTS "${SRC}" OR SRC_CONTENTS_LEN EQUAL 0)
        message(STATUS "    unpacking: ${TAR} -> ${SRC}")
        file(REMOVE_RECURSE ${TMP})
        file(REMOVE_RECURSE ${SRC})
        file(MAKE_DIRECTORY ${TMP})

        execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xfz ${TAR}
            WORKING_DIRECTORY ${TMP}
        )

        file(GLOB CONTENTS "${TMP}/*")

        list(LENGTH CONTENTS LEN)
        if(LEN EQUAL 1)
            if(IS_DIRECTORY ${CONTENTS})
                get_filename_component(CONTENTS ${CONTENTS} ABSOLUTE)
                execute_process(
                    COMMAND ${CMAKE_COMMAND} -E rename ${CONTENTS} ${SRC}
                )
            endif()
        else()
            get_filename_component(CONTENTS ${TMP} ABSOLUTE)
            execute_process(
                COMMAND ${CMAKE_COMMAND} -E rename ${CONTENTS} ${SRC}
            )
        endif()

        file(REMOVE_RECURSE ${TMP})
    endif()
endfunction(download_dependency)
