#!/bin/bash

bash $(pwd)/tools/build.sh

qemu-system-x86_64 --enable-kvm  -vga std -debugcon stdio -hda image.iso "${@}"  
