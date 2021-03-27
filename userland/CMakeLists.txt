add_subdirectory(libcore)

function(add_userland_executable)
    set(options)
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES)
    cmake_parse_arguments(USERLAND_EXEC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_executable(${USERLAND_EXEC_NAME} ${USERLAND_EXEC_SOURCES})

    target_compile_options(${USERLAND_EXEC_NAME} PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${C_COMPILER_FLAGS}>
        $<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILER_FLAGS}>)

    set_property(TARGET ${USERLAND_EXEC_NAME} PROPERTY CXX_STANDARD ${INFOS_CXX_STANDARD})
    set_property(TARGET ${USERLAND_EXEC_NAME} PROPERTY LINK_FLAGS
            "-O3 -nostdlib -nostdinc -static")
endfunction()

add_userland_executable(NAME shell SOURCES shell/main.cpp)
target_link_libraries(shell ${LIBCORE_TARGET})

add_userland_executable(NAME init SOURCES init/main.cpp)
target_link_libraries(init ${LIBCORE_TARGET})

add_userland_executable(NAME cat SOURCES cat/main.cpp)
target_link_libraries(cat ${LIBCORE_TARGET})

add_userland_executable(NAME ls SOURCES ls/main.cpp)
target_link_libraries(ls ${LIBCORE_TARGET})

add_userland_executable(NAME test-graphics SOURCES test-graphics/main.cpp)
target_link_libraries(test-graphics ${LIBCORE_TARGET})

set(NEWLIB_TARGET x86_64-elf-infos CACHE STRING "Newlib target")

ExternalProject_Add(newlib
    GIT_REPOSITORY https://github.com/nankeen/infos-newlib
    GIT_TAG newlib-infos
    GIT_SHALLOW true
    # PATCH_COMMAND ${NEWLIB_PATCH}
    BUILD_IN_SOURCE 0
    SOURCE_DIR ${THIRD_PARTY_DIR}/newlib
    CONFIGURE_COMMAND
        CFLAGS_FOR_TARGET=-ffreestanding
        ${THIRD_PARTY_DIR}/newlib/configure
            --prefix=${TARGET_SYSROOT}
            --target=${NEWLIB_TARGET}
            --disable-multilibs
)
