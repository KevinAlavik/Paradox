#include <system/drivers/rtc.h>
#include <serial/serial.h>

unsigned char read_register(unsigned char dest) {
    __asm__ volatile("cli");
    outb8(RTC_COMMAND, dest);
    return inb8(RTC_DATA);
    __asm__ volatile("sti");
}