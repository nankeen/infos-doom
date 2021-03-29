<h1 align="center">Doom on InfOS</h1>
<p align="center">
    <img alt="GitHub top language" src="https://img.shields.io/github/languages/top/nankeen/infos-doom?style=for-the-badge">
    <img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/nankeen/infos-doom?style=for-the-badge">
    <img alt="GitHub issues" src="https://img.shields.io/github/issues/nankeen/infos-doom?style=for-the-badge">
</p>

> InfOS is the Informatics research operating system, designed specifically for the UG3 Operating Systems course.

This project aims to port Doom over to [InfOS](https://github.com/tspink/infos), the main goal is to extend InfOS with user space graphics.

### Status

Before we can attempt to the port, the kernel must be extended.
The two main hurdles as of 12 Mar 2021 are:

1. Kernel currently uses VGA text mode and does not have framebuffer graphics.
2. User space memory allocation is not supported.

## Gallery

![image](https://user-images.githubusercontent.com/6895854/112836924-1f9e6a80-9093-11eb-8af8-8bdf3102ec5b.png)

### Tasks

- [x] Add multiboot framebuffer graphics support.
- [x] Add memory mapped IO and user space page allocation with `mmap`.
- [x] Add framebuffer console output and font rendering.
- [x] Add user space memory allocator.
- [x] Implement frame drawing function in Doom.
- [ ] Implement player input functions in Doom.
- [ ] Add mouse device support (optional).

## Build

The InfOS project builds with a Makefile.
This project builds the kernel with the original Makefile but uses CMake for other binaries.
To build and generate the a bootable ISO, you would also need GRUB.

### Requirements

Install the dependencies on Ubuntu (ninja is optional)

```bash
sudo apt install grub2 xorriso build-essential ninja-build
```

### Toolchain setup

This project would compile userspace binaries with a different compiler toolchain that needs setup.
Follow https://wiki.osdev.org/GCC_Cross-Compiler to build this, we need to name it `x86_64-elf-infos-*`.

On MacOS you can install `x86_64-elf-gcc` with Homebrew.

```bash
brew install x86_64-elf-gcc
```

Create a directory to store the toolchain, make sure that it is in your path.
This is a quick hack to link the compiler with the correct name.

```bash
mkdir $HOME/opt/bin
for elf_bin in /usr/local/bin/x86_64-elf-*
    do ln $elf_bin ~/opt/bin/x86_64-elf-infos-${elf_bin##*x86_64-elf-}
done
export PATH="$HOME/opt/bin:$PATH"
```

### Build with CMake

This might not be able to build the kernel due to an issue with the toolchain, so if it fails, try compiling the kernel first with `make -C infos/`.

```bash
mkdir build
cd build
cmake -G Ninja ..
cmake --build . --target grub-bootable-iso rootfs
```

### Run
```bash
./run.sh
```
