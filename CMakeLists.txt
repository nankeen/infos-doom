cmake_minimum_required(VERSION 3.14)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain.cmake)

project(infos-doom VERSION 0.0.1 LANGUAGES C CXX ASM)

include(ExternalProject)

set(TARGET_SYSROOT ${CMAKE_BINARY_DIR}/rootfs)
file(MAKE_DIRECTORY ${TARGET_SYSROOT})

if(NOT THIRD_PARTY_DIR)
    set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party CACHE PATH "Third party dir")
endif()

set(NEWLIB_TRIPLE x86_64-elf-infos CACHE STRING "Newlib target")
set(NEWLIB_TARGET newlib)
set(NEWLIB_INSTALL ${TARGET_SYSROOT}/${NEWLIB_TRIPLE})
set(NEWLIB_INCLUDES ${NEWLIB_INSTALL}/include)
set(NEWLIB_LIBS ${NEWLIB_INSTALL}/lib)
if (EXISTS ${NEWLIB_INCLUDES})
    add_custom_target(
        ${NEWLIB_TARGET}
        COMMAND echo "${NEWLIB_INCLUDES} exists, skipping rebuild"
    )
else()
    ExternalProject_Add(${NEWLIB_TARGET}
        GIT_REPOSITORY https://github.com/nankeen/infos-newlib
        GIT_TAG newlib-infos
        GIT_SHALLOW true
        UPDATE_COMMAND ""
        # PATCH_COMMAND ${NEWLIB_PATCH}
        BUILD_IN_SOURCE 0
        SOURCE_DIR ${THIRD_PARTY_DIR}/newlib
        CONFIGURE_COMMAND
            CFLAGS_FOR_TARGET=-ffreestanding
            ${THIRD_PARTY_DIR}/newlib/configure
                --prefix=/
                --target=${NEWLIB_TRIPLE}
                --disable-multilibs
        INSTALL_COMMAND     make DESTDIR=${TARGET_SYSROOT} install
    )
endif()

# Set compilation flags
set(WARNING_FLAGS -Wall -Wextra -Wshadow)
set(COMMON_COMPILER_FLAGS -fno-builtin -fno-stack-protector -I${NEWLIB_INCLUDES} ${WARNING_FLAGS})
set(C_COMPILER_FLAGS ${COMMON_COMPILER_FLAGS})
set(CXX_COMPILER_FLAGS ${COMMON_COMPILER_FLAGS} -fno-exceptions -fno-rtti)
set(INFOS_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

# kernel target name
set(KERNEL_TARGET infos-kernel)
# kernel output path
set(KERNEL_TARGET_OUT ${CMAKE_CURRENT_SOURCE_DIR}/infos/out/${KERNEL_TARGET})
# libcore target name
set(LIBCORE_TARGET core)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}")

# Define kernel compile target
add_custom_command(OUTPUT ${KERNEL_TARGET_OUT}
    COMMAND make cc=${CMAKE_C_COMPILER} cxx=${CMAKE_CXX_COMPILER} objcopy=${CMAKE_OBJCOPY} ld=${CMAKE_LINKER} as=${CMAKE_ASM_COMPILER}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/infos)
add_custom_target(${KERNEL_TARGET} DEPENDS ${KERNEL_TARGET_OUT})

# Add userland targets
add_subdirectory(userland)

# Define grub ISO target
find_program(GRUB_MKRESCUE grub-mkrescue)
if (NOT GRUB_MKRESCUE)
    message(WARNING "Unable to find `grub-mkrescue`. Bootable ISO image generation will not be available.")
else ()
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/isofiles/boot/grub)
    file(COPY ${CMAKE_SOURCE_DIR}/boot/grub.cfg DESTINATION ${CMAKE_BINARY_DIR}/isofiles/boot/grub)
    set(ISO_TARGET grub-bootable-iso)
    set(ISO_PATH ${CMAKE_BINARY_DIR}/infos.iso)
    add_custom_target(${ISO_TARGET} ALL
        COMMAND ${CMAKE_COMMAND} -E copy ${KERNEL_TARGET_OUT} ${CMAKE_BINARY_DIR}/isofiles/boot/
        COMMAND ${GRUB_MKRESCUE} -o ${ISO_PATH} ${CMAKE_BINARY_DIR}/isofiles
        DEPENDS ${KERNEL_TARGET})
endif()

# Define rootfs compile target
find_program(TAR_CMD tar)
if (NOT TAR_CMD)
    message (WARNING "Unable to find `tar`. Filesystem generation will not be available.")
else()
    set(FS_TARGET rootfs)
    set(FS_BINS init shell cat ls test-graphics test_newlib)
    set(FS_PATH ${TARGET_SYSROOT}.tar)
    add_custom_target(${FS_TARGET} ALL
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:init> ${TARGET_SYSROOT}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:shell> ${TARGET_SYSROOT}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:cat> ${TARGET_SYSROOT}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ls> ${TARGET_SYSROOT}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:test-graphics> ${TARGET_SYSROOT}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:test_newlib> ${TARGET_SYSROOT}
        COMMAND ${TAR_CMD} cf ${FS_PATH} -C ${TARGET_SYSROOT} ${FS_BINS})

    # Create qemu compile target
    find_program(QEMU qemu-system-x86_64)
    if (QEMU AND ISO_TARGET)
        set(RUN_QEMU_TARGET run-qemu)
        set(QEMU_ARGS -cdrom ${ISO_PATH} -m 6G -debugcon stdio -hda ${FS_PATH})
        if (QEMU_USE_GDB)
            set(QEMU_ARGS ${QEMU_ARGS} -s -S)
        endif()
        if (QEMU_USE_VNC)
            set(QEMU_ARGS ${QEMU_ARGS} -display vnc=localhost:1337)
        endif()

        add_custom_target(${RUN_QEMU_TARGET} COMMAND ${QEMU} ${QEMU_ARGS})
        add_dependencies(${RUN_QEMU_TARGET} ${ISO_TARGET})
    endif ()
endif()
