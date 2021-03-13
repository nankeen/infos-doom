cmake_minimum_required(VERSION 3.14)

project(infos-doom VERSION 0.0.1)

include(ExternalProject)

# Set compilation flags
set (WARNING_FLAGS -Werror -Wall -Wextra -Wshadow)
set (COMMON_COMPILER_FLAGS -ffreestanding -fno-builtin -fno-stack-protector ${WARNING_FLAGS})
set (C_COMPILER_FLAGS ${COMMON_COMPILER_FLAGS})
set (CXX_COMPILER_FLAGS ${COMMON_COMPILER_FLAGS} -fno-exceptions -fno-rtti)
set (INFOS_CXX_STANDARD 20)
set (CMAKE_C_STANDARD 11)

# kernel target name
set(KERNEL_TARGET infos-kernel)
# kernel output path
set(KERNEL_TARGET_OUT ${CMAKE_CURRENT_SOURCE_DIR}/infos/out/${KERNEL_TARGET})
# libcore target name
set (LIBCORE_TARGET core)

add_subdirectory(userland)

# Define kernel compile target
add_custom_command(OUTPUT ${KERNEL_TARGET_OUT}
                   COMMAND make
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/infos)
add_custom_target(${KERNEL_TARGET} DEPENDS ${KERNEL_TARGET_OUT})

# Define grub ISO target
find_program(GRUB_MKRESCUE grub-mkrescue)
if (NOT GRUB_MKRESCUE)
    message (WARNING "Unable to find `grub-mkrescue`. Bootable ISO image generation will not be available.")
else ()
    file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/isofiles/boot/grub)
    file (COPY ${CMAKE_SOURCE_DIR}/boot/grub.cfg DESTINATION ${CMAKE_BINARY_DIR}/isofiles/boot/grub)
    set (ISO_TARGET grub-bootable-iso)
    set (ISO_PATH ${CMAKE_BINARY_DIR}/infos.iso)
    add_custom_target(${ISO_TARGET}
        COMMAND ${CMAKE_COMMAND} -E copy ${KERNEL_TARGET_OUT} ${CMAKE_BINARY_DIR}/isofiles/boot/
        COMMAND ${GRUB_MKRESCUE} -o ${ISO_PATH} ${CMAKE_BINARY_DIR}/isofiles
        DEPENDS ${KERNEL_TARGET})
endif()

find_program(TAR_CMD tar)
if (NOT TAR_CMD)
    message (WARNING "Unable to find `tar`. Filesystem generation will not be available.")
else()
    file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/rootfs)
    set(FS_TARGET rootfs)
    set(FS_BINS init shell cat ls)
    add_custom_target(${FS_TARGET}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:init> ${CMAKE_BINARY_DIR}/rootfs
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:shell> ${CMAKE_BINARY_DIR}/rootfs
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:cat> ${CMAKE_BINARY_DIR}/rootfs
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ls> ${CMAKE_BINARY_DIR}/rootfs
        COMMAND ${TAR_CMD} cf ${CMAKE_BINARY_DIR}/rootfs.tar -C ${CMAKE_BINARY_DIR}/rootfs ${FS_BINS})
endif()
