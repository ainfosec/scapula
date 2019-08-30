unset(AARCH32_FLAGS)
unset(AARCH32_C_FLAGS)
unset(AARCH32_CXX_FLAGS)

list(APPEND AARCH32_FLAGS
    -fpic
    -fno-stack-protector 
    -fno-builtin
    -fno-common
    -ffreestanding
    -fno-omit-frame-pointer
    -mlittle-endian
    -march=armv7-a
    -isystem ${CMAKE_INSTALL_PREFIX}/include
)

list(APPEND AARCH32_C_FLAGS
    -std=gnu11
)

list(APPEND AARCH32_CXX_FLAGS
    -x c++
    -std=c++17
    # -isystem ${CMAKE_INSTALL_PREFIX}/include/c++/v1
)
