# Paradox
A x86_64 operating system built ontop of the limine bootloader.

Project board: https://github.com/users/KevinAlavik/projects/5

![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/KevinAlavik/Paradox/main.yml?branch=main&label=Daily%20ISO%20Build)

![GitHub last commit (branch)](https://img.shields.io/github/last-commit/kevinalavik/paradox/main?label=Last%20Commit)
![GitHub commit activity](https://img.shields.io/github/commit-activity/t/KevinAlavik/Paradox)
![GitHub repo size](https://img.shields.io/github/repo-size/KevinAlavik/Paradox)

![GitHub watchers](https://img.shields.io/github/watchers/KevinAlavik/Paradox)
![GitHub Repo stars](https://img.shields.io/github/stars/KevinAlavik/Paradox)


## Project ideas or Questions?
Got any ideas or got qeuestions for Paradox post them [here](https://github.com/KevinAlavik/Paradox/discussions)


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
 
Some authors have been credited on the first line in some files!

## License
Paradox is licensed under the [GPL v3](https://www.gnu.org/licenses/gpl-3.0.html) license. 

![image](https://github.com/KevinAlavik/Paradox/assets/95900603/eff41817-8071-407a-9b8b-942928756c33)

