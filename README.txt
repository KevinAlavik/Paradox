Paradox

A x86_64 operating system built on top of the Limine bootloader.

Building:

To build a bootable disk file simply use our little build script:

./tools/build.sh

Now you should have an image.iso file in the Paradox directory. To boot this using QEMU, you can use the following command:

qemu-system-x86_64 -vga std -debugcon stdio -audiodev coreaudio,id=audio0 -machine pcspk-audiodev=audio0 -hda image.iso

This will run QEMU with kmesg on the stdio stream (on your host) and adds support for PC Speaker, allowing sound.

You could also use our custom boot script to build and boot using QEMU:

./tools/boot.sh

Credits:

- schkwve
  - Made nighterm-extended (https://github.com/schkwve/nighterm-extended)
