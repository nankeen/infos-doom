<h1 align="center">Doom on InfOS</h1>
<p align="center">
    <img alt="GitHub top language" src="https://img.shields.io/github/languages/top/nankeen/infos?style=for-the-badge">
    <img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/nankeen/infos?style=for-the-badge">
    <img alt="GitHub issues" src="https://img.shields.io/github/issues/nankeen/infos-doom?style=for-the-badge">
</p>

> InfOS is the Informatics research operating system, designed specifically for the UG3 Operating Systems course.

This project aims to port Doom over to [InfOS](https://github.com/tspink/infos), the main goal is to extend InfOS with user space graphics.

### Status

Before we can attempt to the port, the kernel must be extended.
The two main hurdles as of 12 Mar 2021 are:

1. Kernel currently uses VGA text mode and does not have framebuffer graphics.
2. User space memory allocation is not supported.

### Tasks

- [x] Add multiboot framebuffer graphics support.
- [x] Add memory mapped IO and user space page allocation with `mmap`.
- [x] Add framebuffer console output and font rendering.
- [ ] Add user space memory allocator.
- [ ] Implement frame drawing function in Doom.
- [ ] Implement player input functions in Doom.
- [ ] Add mouse device support (optional).

## Build

The InfOS project builds with a Makefile.
This project builds the kernel with the original Makefile but uses CMake for other binaries.
To build and generate the a bootable ISO, you would also need GRUB.

Install the dependencies on Ubuntu (ninja is optional)

```bash
sudo apt install grub2 xorriso build-essential ninja-build
```

Build with CMake
```bash
mkdir build
cd build
cmake -G Ninja ..
cmake --build . --target grub-bootable-iso rootfs
```

Run
```bash
./run.sh
```
