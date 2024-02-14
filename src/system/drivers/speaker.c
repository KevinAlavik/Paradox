#include "speaker.h"
#include <system/pit/pit.h>
#include <serial/serial.h>

void play_sound(uint32_t nFrequence)
{
    uint32_t Div;
    uint8_t tmp;

    Div = 1193180 / nFrequence;
    outb8(0x43, 0xb6);
    outb8(0x42, (uint8_t)(Div));
    outb8(0x42, (uint8_t)(Div >> 8));

    tmp = inb8(0x61);
    if (tmp != (tmp | 3))
    {
        outb8(0x61, tmp | 3);
    }
}

void nosound()
{
    uint8_t tmp = inb8(0x61) & 0xFC;

    outb8(0x61, tmp);
}

void beep()
{
    play_sound(1000);
    pit_sleep(10000);
    nosound();
}