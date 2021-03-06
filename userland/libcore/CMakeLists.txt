if (NOT DEFINED LIBCORE_TARGET)
    set(LIBCORE_TARGET libcore)
    message (WARNING "LIBCORE_TARGET was not set by project root. Defaulting to '${LIBCORE_TARGET}'.")
    set(LIBCORE_TARGET ${LIBCORE_TARGET} PARENT_SCOPE)
endif ()

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*.cpp")

add_library(${LIBCORE_TARGET} STATIC ${SOURCES})
target_include_directories(${LIBCORE_TARGET} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
set_property(TARGET ${LIBCORE_TARGET} PROPERTY CXX_STANDARD ${INFOS_CXX_STANDARD})
set(LIBCORE_C_COMPILER_FLAGS ${C_COMPILER_FLAGS} -Wno-unused-function)
set(LIBCORE_CXX_COMPILER_FLAGS ${CXX_COMPILER_FLAGS} -Wno-unused-function)
target_compile_options(${LIBCORE_TARGET} PRIVATE
    $<$<COMPILE_LANGUAGE:C>:${LIBCORE_C_COMPILER_FLAGS}>
    $<$<COMPILE_LANGUAGE:CXX>:${LIBCORE_CXX_COMPILER_FLAGS}>)
