function(add_userland_executable)
    set(options)
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES)
    cmake_parse_arguments(USERLAND_EXEC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_executable(${USERLAND_EXEC_NAME} ${USERLAND_EXEC_SOURCES})
    add_dependencies(${USERLAND_EXEC_NAME} ${NEWLIB_TARGET})

    target_compile_options(${USERLAND_EXEC_NAME} PRIVATE
        $<$<COMPILE_LANGUAGE:C>:${C_COMPILER_FLAGS}>
        $<$<COMPILE_LANGUAGE:CXX>:${CXX_COMPILER_FLAGS}>)
    target_include_directories(${USERLAND_EXEC_NAME} PUBLIC ${NEWLIB_INCLUDES})
    target_link_directories(${USERLAND_EXEC_NAME} PRIVATE ${NEWLIB_LIBS})
    target_link_libraries(${USERLAND_EXEC_NAME} c g m nosys)

    set_property(TARGET ${USERLAND_EXEC_NAME} PROPERTY CXX_STANDARD ${INFOS_CXX_STANDARD})
    set_property(TARGET ${USERLAND_EXEC_NAME} PROPERTY LINK_FLAGS
                 "-O3 -nostdlib -nostdinc -static ${NEWLIB_LIBS}/crt0.o")
endfunction()

function(add_core_executable)
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
    target_link_libraries(${USERLAND_EXEC_NAME} ${LIBCORE_TARGET})
endfunction()

set(DOOM_TARGET doom)

add_subdirectory(libcore)
add_subdirectory(doom)

add_core_executable(NAME shell SOURCES shell/main.cpp)
add_core_executable(NAME init SOURCES init/main.cpp)
add_core_executable(NAME cat SOURCES cat/main.cpp)
add_core_executable(NAME ls SOURCES ls/main.cpp)
add_core_executable(NAME test-graphics SOURCES test-graphics/main.cpp)

add_userland_executable(NAME test-newlib SOURCES test-newlib/main.cpp)
