#include <filesystem/tar.h>
#include <entry/init.h>
#include <kif.h>
#include <nighterm/nighterm.h> // Dont make this need nighterm.h to draw things on the screen. Use vga.h
#include <printf.h>
#include <stdint.h>
#include <x86_64/cpu/cpu.h>
#include <system/devices/speaker.h>
#include <system/memory/pmm.h>
#include <vga.h>
#include <tty/tty.h>

void panic(const char *reason, int_frame_t frame)
{
        printf("\n\n* %s\n\n", reason);

        printf("rax: 0x%.16llX, rbx: 0x%.16llX, rcx: 0x%.16llX, rdx: 0x%.16llX\n",
               frame.rax, frame.rbx, frame.rcx, frame.rdx);

        printf("rsp: 0x%.16llX, rbp: 0x%.16llX, rsi: 0x%.16llX, rdi: 0x%.16llX\n",
               frame.rsp, frame.rbp, frame.rsi, frame.rdi);

        printf("r8:  0x%.16llX, r9:  0x%.16llX, r10: 0x%.16llX, r11: 0x%.16llX\n",
               frame.r8, frame.r9, frame.r10, frame.r11);

        printf("r12: 0x%.16llX, r13: 0x%.16llX, r14: 0x%.16llX, r15: 0x%.16llX\n",
               frame.r12, frame.r13, frame.r14, frame.r15);

        printf("rfl: 0x%.16llX, rip: 0x%.16llX, cs:  0x%.16llX, ss:  0x%.16llX\n",
               frame.rflags, frame.rip, frame.cs, frame.ss);

        dprintf("[\e[0;31mKernel Panic\e[0m] rax: 0x%.16llX, rbx: 0x%.16llX, rcx: "
                "0x%.16llX, rdx: "
                "0x%.16llX\n",
                frame.rax, frame.rbx, frame.rcx, frame.rdx);
        dprintf("[\e[0;31mKernel Panic\e[0m] rsp: 0x%.16llX, rbp: 0x%.16llX, rsi: "
                "0x%.16llX, rdi: "
                "0x%.16llX\n",
                frame.rsp, frame.rbp, frame.rsi, frame.rdi);
        dprintf("[\e[0;31mKernel Panic\e[0m] r8:  0x%.16llX, r9:  0x%.16llX, r10: "
                "0x%.16llX, r11: "
                "0x%.16llX\n",
                frame.r8, frame.r9, frame.r10, frame.r11);
        dprintf("[\e[0;31mKernel Panic\e[0m] r12: 0x%.16llX, r13: 0x%.16llX, r14: "
                "0x%.16llX, r15: "
                "0x%.16llX\n",
                frame.r12, frame.r13, frame.r14, frame.r15);
        dprintf("[\e[0;31mKernel Panic\e[0m] rfl: 0x%.16llX, rip: 0x%.16llX, cs:  "
                "0x%.16llX, ss:  "
                "0x%.16llX\n",
                frame.rflags, frame.rip, frame.cs, frame.ss);

        // visualize_pmm(0, 0, framebuffer->width, (bitmap_size * PAGE_SIZE) /
        // framebuffer->width);

        beep();
}
