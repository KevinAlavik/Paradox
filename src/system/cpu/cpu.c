#include <system/cpu/cpu.h>
#include <serial/serial.h>

void hcf()
{
    asm("cli");
    for (;;)
    {
        asm("hlt");
    }
}

void hlt()
{
    for (;;)
    {
        asm("hlt");
    }
}

void cli()
{
    asm("cli");
}

void reboot()
{
    char good = 0x02;
    while (good & 0x02)
        good = inb8(0x64);
    outb8(0x64, 0xFE);
    asm volatile("cli");
    asm volatile("hlt");
}

void shutdown_vm_only()
{
    outb16(0x604, 0x2000);  // qemu
    outb16(0xB004, 0x2000); // bochs
    outb16(0x4004, 0x3400); // virtualbox
    asm volatile("cli");
    asm volatile("hlt");
}

void shutdown()
{
    shutdown_vm_only();
}