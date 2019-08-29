# Init Subproject
#
# Initializes a sub project that was added to the build system through a
# higher-level super build
#

include(${CMAKE_CURRENT_LIST_DIR}/macros/macros.cmake)

string(REPLACE ";" " " CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include_directories(SYSTEM ${CMAKE_INSTALL_PREFIX}/include)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)

get_cmake_property(_vars CACHE_VARIABLES)
foreach (_var ${_vars})
    set(${_var} ${${_var}})
endforeach()
