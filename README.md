# Paradox
A x86_64 operating system built ontop of the limine bootloader.

## Building
To build a bootable disk file simply use our little build script:
```bash
./tools/build.sh
```
Now you should have a **image.iso** in the Paradox directory. To boot this using QEMU you can use the following command
```bash
qemu-system-x86_64 -vga std -debugcon stdio -audiodev coreaudio,id=audio0 -machine pcspk-audiodev=audio0 -hda image.iso
```
This will run qemu with **kmesg** on the stdio stream (on your host) and this also adds support for PC Speaker, so we can make sound.

You could also use our custom boot script to build and boot using QEMU:
```bash
./tools/boot.sh
```
## Credits

- [schkwve](https://github.com/schkwve)
  - Made [**nighterm-extended**](https://github.com/schkwve/nighterm-extended)
