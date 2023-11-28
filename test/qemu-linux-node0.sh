#!/bin/bash

_SCRIPT_DIR=$(dirname -- $0)

# Should have called the directory linux-torvalds, ah well.
# Be careful not to drop serial port driver support!
_KERNEL_DIR="${_SCRIPT_DIR}/bzimage/bzImage.0"

# BusyBox + glibc.
_ROOTFS_DIR="${_SCRIPT_DIR}/rootfs-img/rootfs.ext4.0"

# Number of processes usable by VM.
_SMP=2

printf "Using kernel @ %s\n" $_KERNEL_DIR
printf "Using rootfs @ %s\n" $_ROOTFS_DIR
printf "Press any key to continue...\n"
read -n 1 -r &>/dev/null

qemu-system-x86_64 \
    -kernel $_KERNEL_DIR \
    -append "root=/dev/vda console=ttyS0 earlyprintk=serial nokaslr" \
    -nographic \
    -drive file=$_ROOTFS_DIR,if=virtio,index=0,media=disk,format=raw \
    -m 512M \
    -enable-kvm \
    -cpu host \
    -smp $_SMP \
    -nic bridge,br=virbr0,model=e1000 \

# `-s` enables gdb listening at :1234
# `-S` stops emulator from running unless gdb listens
