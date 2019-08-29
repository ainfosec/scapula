set(_PROJECT_CONFIG_DIR ${CMAKE_SOURCE_DIR}/scripts/cmake/config)
macro(include_external_config)
    # Preference 1: -DCONFIG specified on CMake command line
    if(CONFIG)
        foreach(c ${CONFIG})
            if(EXISTS "${_PROJECT_CONFIG_DIR}/${c}.cmake")
                message(STATUS "Config: ${_PROJECT_CONFIG_DIR}/${c}.cmake")
                include(${_PROJECT_CONFIG_DIR}/${c}.cmake)
                continue()
            endif()
            if(NOT IS_ABSOLUTE "${c}")
                get_filename_component(c "${BUILD_ROOT_DIR}/${c}" ABSOLUTE)
            endif()
            if(EXISTS "${c}")
                message(STATUS "Config: ${c}")
                include(${c})
                continue()
            endif()

            message(FATAL_ERROR "File not found: ${c}")
        endforeach(c)

    # Preference 2: -DCONFIG not specified, but file named config.cmake exists
    # one directory up from the project
    elseif(EXISTS "${CMAKE_SOURCE_DIR}/../config.cmake")
        get_filename_component(CONFIG "${CMAKE_SOURCE_DIR}/../config.cmake" ABSOLUTE)
        message(STATUS "Config: ${CONFIG}")
        include(${CONFIG})

    # Preference 3: No config, use cmake-gui/ccmake
    else()
        message(STATUS "Config: using default options with no config file")
        message(STATUS "    configure build options using the command: ccmake .")
    endif()
endmacro(include_external_config)
