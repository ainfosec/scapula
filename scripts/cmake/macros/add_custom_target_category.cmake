function(add_custom_target_category TEXT)
    add_custom_command(
        TARGET info
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color " "
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green "${TEXT}:"
    )
endfunction(add_custom_target_category)
