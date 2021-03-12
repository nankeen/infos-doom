#!/bin/sh

TOP=`pwd`
INFOS_DIR=$TOP/infos
INFOS_USER_DIR=$TOP/infos-user
ROOTFS=$INFOS_USER_DIR/bin/rootfs.tar
ISO=$TOP/build/infos.iso
QEMU=qemu-system-x86_64

$QEMU -cdrom $ISO -m 6G -debugcon stdio -hda $ROOTFS -display vnc=localhost:1234 -S -s
