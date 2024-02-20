# Paradox
A x86_64 operating system built ontop of the limine bootloader.

Project board: https://github.com/users/KevinAlavik/projects/5

![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/KevinAlavik/Paradox/main.yml?branch=main&label=Daily%20ISO%20Build)
![GitHub last commit (branch)](https://img.shields.io/github/last-commit/kevinalavik/paradox/main?label=Last%20Commit)
![GitHub watchers](https://img.shields.io/github/watchers/KevinAlavik/Paradox)

## Project ideas or Questions?
Got any ideas or got qeuestions for Paradox post them [here](https://github.com/KevinAlavik/Paradox/discussions)

## Contributing
We always want people to contribute, but make sure to follow these things:
- No confusing names (like make function names be called something easy to understand).
- Put things in the correct paths:
  - corelib/
     - Put kernel libraries here.
  - kernel/
     - Avoid putting any thing in here.
  - . (in the src)
     - Here are the important things placed, like nighterm, serial, filesystem. These will be moved later on when i do a refactor.
  - system/
     -  Put nearly evertyhing in here in respective folders, drivers: Drivers for thing, keyboard, speaker, etc.

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

## License
Paradox is licensed under the [GPL v3](https://www.gnu.org/licenses/gpl-3.0.html) license. 

![image](https://github.com/KevinAlavik/Paradox/assets/95900603/eff41817-8071-407a-9b8b-942928756c33)

