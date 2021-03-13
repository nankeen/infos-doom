#!/bin/sh

TOP=`pwd`
ROOTFS=$TOP/build/rootfs.tar
ISO=$TOP/build/infos.iso
QEMU=qemu-system-x86_64

$QEMU -cdrom $ISO -m 6G -debugcon stdio -hda $ROOTFS -s
