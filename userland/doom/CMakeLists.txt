if (NOT DEFINED DOOM_TARGET)
    set(DOOM_TARGET doom)
    message (WARNING "DOOM_TARGET was not set by project root. Defaulting to '${DOOM_TARGET}'.")
    set(DOOM_TARGET ${DOOM_TARGET} PARENT_SCOPE)
endif ()

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*.c")

add_executable(${DOOM_TARGET} ${SOURCES})
add_dependencies(${DOOM_TARGET} ${NEWLIB_TARGET})

set_property(TARGET ${DOOM_TARGET} PROPERTY LINK_FLAGS
             "-Wl,--gc-sections -nostdlib -nostdinc -static ${NEWLIB_LIBS}/crt0.o")

target_include_directories(${DOOM_TARGET} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src ${NEWLIB_INCLUDES})
target_link_directories(${DOOM_TARGET} PRIVATE ${NEWLIB_LIBS})
target_link_libraries(${DOOM_TARGET} c g m nosys)

set(DOOM_C_COMPILER_FLAGS -ggdb3 -D_DEFAULT_SOURCE)
target_compile_options(${DOOM_TARGET} PRIVATE $<$<COMPILE_LANGUAGE:C>:${DOOM_C_COMPILER_FLAGS}>)
