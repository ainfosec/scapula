unset(AARCH64_FLAGS)
unset(AARCH64_C_FLAGS)
unset(AARCH64_CXX_FLAGS)

list(APPEND AARCH64_FLAGS
    -fpie
    -fno-stack-protector 
    -fno-builtin
    -fno-common
    -ffreestanding
    -fno-omit-frame-pointer
    -mlittle-endian
    -mstrict-align
    -march=armv8-a
    -isystem ${CMAKE_INSTALL_PREFIX}/include
)

list(APPEND AARCH64_C_FLAGS
    -std=gnu11
)

list(APPEND AARCH64_CXX_FLAGS
    -x c++
    -std=c++17
    # -isystem ${CMAKE_INSTALL_PREFIX}/include/c++/v1
)
