function(add_custom_target_info)
    set(oneVal TARGET COMMENT)
    cmake_parse_arguments(ARG "" "${oneVal}" "" ${ARGN})

    if(NOT_ARG_TARGET)
        right_justify("${BUILD_COMMAND}" 20 JUSTIFY_STR)
        add_custom_command(
            TARGET info
            COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --yellow --no-newline "    ${BUILD_COMMAND}"
            COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --red --no-newline "${JUSTIFY_STR}- "
            COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white "${ARG_COMMENT}"
        )
    else()
        right_justify("${BUILD_COMMAND} ${ARG_TARGET}" 20 JUSTIFY_STR)
        add_custom_command(
            TARGET info
            COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --yellow --no-newline "    ${BUILD_COMMAND} ${ARG_TARGET}"
            COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --red --no-newline "${JUSTIFY_STR}- "
            COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --white "${ARG_COMMENT}"
        )
    endif()
endfunction(add_custom_target_info)
