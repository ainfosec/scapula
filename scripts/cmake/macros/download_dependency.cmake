# Download Dependency
#
# Downloads a dependency from a URL. Dependencies can either be
# a tarball or a zip file. These downloaded files are placeed in the CACHE_DIR.
# If the provided MD5 hash does not match, the cached download is redownloaded.
#
# @param NAME the name of the dependency
# @param URL The URL for the dependency
# @param URL_MD5 The MD5 of the file being downloaded
#
function(download_dependency NAME)
    set(oneVal URL URL_MD5 GIT_REPOSITORY GIT_TAG PREFIX)
    cmake_parse_arguments(ARG "" "${oneVal}" "" ${ARGN})

    set(SRC ${CACHE_DIR}/${NAME})

    if(ARG_URL)
        if(NOT ARG_URL_MD5)
            message(FATAL_ERROR "Invalid URL MD5: ${ARG_URL_MD5}")
        endif()

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

        set(TMP ${CACHE_DIR}/${NAME}_tmp)
        set(TAR ${CACHE_DIR}/${NAME}${EXT})

        # TODO
        #
        # If a dependency needs to be downloaded, currently, we remove the
        # source directory which forces a recompile. We need to verify that
        # when this happens, all of the targets that rely on this dependency
        # are also recompiled / relinked.
        #

        foreach(ATTEMPT RANGE 1 5 1)
            if(EXISTS "${TAR}")
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
            endif()
            message(STATUS "    download file: ${Green}complete${ColorReset}")
        endforeach()

        if(EXISTS ${TAR})
            file(MD5 ${TAR} MD5)
            if(NOT "${MD5}" STREQUAL "${ARG_URL_MD5}")
                message(FATAL_ERROR "Failed to download ${ARG_URL} with md5 hash of ${ARG_URL_MD5}")
            endif()
        else()
            message(FATAL_ERROR "Failed to download ${ARG_URL} with md5 hash of ${ARG_URL_MD5}")
        endif()

        if(NOT EXISTS "${SRC}")
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
    endif()

    if(ARG_GIT_REPOSITORY)
        if(ARG_GIT_TAG)
            set(ARG_GIT_TAG -b ${ARG_GIT_TAG})
        endif()
        if(NOT EXISTS "${SRC}")
            execute_process(COMMAND git clone ${ARG_GIT_REPOSITORY} ${ARG_GIT_TAG} ${SRC})
        endif()
    endif()

endfunction(download_dependency)
