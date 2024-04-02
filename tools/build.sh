#!/bin/bash
OS_NAME="Paradox"

make -C limine

if [[ "$OSTYPE" == "darwin"* ]]; then
    make CC=x86_64-elf-gcc LD=x86_64-elf-ld
else
    make 
fi

mkdir -p iso_root
mkdir -p modules

tar -cvf modules/ramdisk.tar initrd/*

# PARADOX_VER=0.1.1
# echo -n "Build Version: Paradox v$(echo $PARADOX_VER), Build Date: $(date "+%A (%m/%d/%Y) %H:%M")" > modules/build_info.txt

cp -v bin/"$OS_NAME" modules/* limine.cfg limine/limine-bios.sys \
    limine/limine-bios-cd.bin limine/limine-uefi-cd.bin iso_root/

mkdir -p iso_root/EFI/BOOT
cp -v limine/BOOTX64.EFI iso_root/EFI/BOOT/
cp -v limine/BOOTIA32.EFI iso_root/EFI/BOOT/


xorriso -as mkisofs -b limine-bios-cd.bin \
    -no-emul-boot -boot-load-size 4 -boot-info-table \
    --efi-boot limine-uefi-cd.bin \
    -efi-boot-part --efi-boot-image --protective-msdos-label \
    iso_root -o image.iso

./limine/limine bios-install image.iso
rm -rf iso_root
