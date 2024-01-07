#!/bin/bash

# Compile test.c
x86_64-elf-gcc -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-lto -fPIE -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-red-zone -I src -I limine -I src/nighterm -I src/corelib -g -c test.c -o test.o

# Compile nighterm.c and all .c files in subdirectories
find src/nighterm -type f -name "*.c" -exec x86_64-elf-gcc -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-lto -fPIE -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-red-zone -I src -I limine -I src/nighterm -I src/corelib -g -c {} \; -exec basename {} \; | sed 's/\.c//' | xargs -I {} mv {} src/nighterm

# Compile serial.c
x86_64-elf-gcc -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-lto -fPIE -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-red-zone -I src -I limine -I src/nighterm -I src/corelib -g -c src/serial/serial.c -o serial.o

# Compile printf.c and cpu.c
x86_64-elf-gcc -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-lto -fPIE -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-red-zone -I src -I limine -I src/nighterm -I src/corelib -g -c src/corelib/printf.c -o printf.o
x86_64-elf-gcc -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-lto -fPIE -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-red-zone -I src -I limine -I src/nighterm -I src/corelib -g -c src/system/cpu/cpu.c -o cpu.o

# Link all object files into the test executable
x86_64-elf-ld -m elf_x86_64 -nostdlib -static -pie --no-dynamic-linker -z text -z max-page-size=0x1000 -T linker.ld -g test.o src/nighterm/*.o serial.o printf.o cpu.o -o test

# Remove the generated .o files
rm -rf src/nighterm/*.o

rm -rf ./*.o
