if(ENABLE_BUILD_SCAPULA AND NOT WIN32)
    message(STATUS "Including dependency: libfdt")

    set(LIBFDT_SOURCE_DIR ${CACHE_DIR}/dtc)
    set(LIBFDT_BUILD_DIR ${DEPENDS_DIR}/libfdt/${VMM_PREFIX}/build)
    set(LIBFDT_C_COMPILER /usr/bin/aarch64-linux-gnu-gcc)

    string(CONCAT LIBFDT_C_FLAGS
        "-Wpointer-arith "
        "-Wcast-qual "
        "-Wnested-externs "
        "-Wstrict-prototypes "
        "-Wmissing-prototypes "
        "-Wredundant-decls "
        "-Wshadow "
        "-Werror "
        "-fno-stack-protector "
        "-mgeneral-regs-only "
        "-fno-common "
        "-fno-builtin "
        "-ffreestanding "
        "-std=gnu99 "
    )

    if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        string(CONCAT LIBFDT_C_FLAGS "-O3 ")
    else()
        string(CONCAT LIBFDT_C_FLAGS "-Os -g ")
    endif()

    download_dependency(
        libfdt
        URL          ${DTC_URL}
        URL_MD5      ${DTC_URL_MD5}
    )

    add_dependency(
        libfdt vmm
        DEPENDS dtc_${VMM_PREFIX}
        CONFIGURE_COMMAND   ${CMAKE_COMMAND} -E copy_directory ${LIBFDT_SOURCE_DIR} ${LIBFDT_BUILD_DIR}
        BUILD_COMMAND
            make -C ${LIBFDT_BUILD_DIR} libfdt
            NO_PYTHON=1
            CC=${LIBFDT_C_COMPILER}
            CFLAGS=${LIBFDT_C_FLAGS}
        INSTALL_COMMAND
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${LIBFDT_BUILD_DIR}/libfdt/libfdt.a ${VMM_PREFIX_PATH}/lib/libfdt.a
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${LIBFDT_BUILD_DIR}/libfdt/fdt.h ${VMM_PREFIX_PATH}/include/fdt.h
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${LIBFDT_BUILD_DIR}/libfdt/libfdt.h ${VMM_PREFIX_PATH}/include/libfdt.h
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${LIBFDT_BUILD_DIR}/libfdt/libfdt_env.h ${VMM_PREFIX_PATH}/include/libfdt_env.h
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${LIBFDT_BUILD_DIR}/libfdt/libfdt_internal.h ${VMM_PREFIX_PATH}/include/libfdt_internal.h
    )

endif()
