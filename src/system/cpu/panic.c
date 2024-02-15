#include <stdint.h>
#include <system/cpu/panic.h>
#include <kernel/boot.h>
#include <nighterm/backends/vga.h>
#include <nighterm/nighterm.h>
#include <system/drivers/speaker.h>
#include <system/memory/pmm.h>

#include <printf.h>
#include <kif.h>

void panic(const char *reason, int_frame_t frame)
{
    nighterm_clear();
    term.cy = 18;
    term.cx = 4;

    nighterm_flush(70, 105, 255);
    nighterm_set_char_bg(70, 105, 255);
    draw_image((char *)mod_request.response->modules[3]->address, 0, 0, 0);

    printf("Computer no workie :( \n\n");
    term.cx = 4;
    printf("* %s\n\n", reason);

    nighterm_set_char_fg(201, 183, 193);
    term.cx = 4;
    printf("rax: 0x%.16llX, rbx: 0x%.16llX, rcx: 0x%.16llX, rdx: 0x%.16llX\n", frame.rax, frame.rbx, frame.rcx, frame.rdx);
    term.cx = 4;
    printf("rsp: 0x%.16llX, rbp: 0x%.16llX, rsi: 0x%.16llX, rdi: 0x%.16llX\n", frame.rsp, frame.rbp, frame.rsi, frame.rdi);
    term.cx = 4;
    printf("r8:  0x%.16llX, r9:  0x%.16llX, r10: 0x%.16llX, r11: 0x%.16llX\n", frame.r8, frame.r9, frame.r10, frame.r11);
    term.cx = 4;
    printf("r12: 0x%.16llX, r13: 0x%.16llX, r14: 0x%.16llX, r15: 0x%.16llX\n", frame.r12, frame.r13, frame.r14, frame.r15);
    term.cx = 4;
    printf("rfl: 0x%.16llX, rip: 0x%.16llX, cs:  0x%.16llX, ss:  0x%.16llX\n", frame.rflags, frame.rip, frame.cs, frame.ss);
    term.cx = 4;

    dprintf("[KERNEL PANIC] rax: 0x%.16llX, rbx: 0x%.16llX, rcx: 0x%.16llX, rdx: 0x%.16llX\n", frame.rax, frame.rbx, frame.rcx, frame.rdx);
    dprintf("[KERNEL PANIC] rsp: 0x%.16llX, rbp: 0x%.16llX, rsi: 0x%.16llX, rdi: 0x%.16llX\n", frame.rsp, frame.rbp, frame.rsi, frame.rdi);
    dprintf("[KERNEL PANIC] r8:  0x%.16llX, r9:  0x%.16llX, r10: 0x%.16llX, r11: 0x%.16llX\n", frame.r8, frame.r9, frame.r10, frame.r11);
    dprintf("[KERNEL PANIC] r12: 0x%.16llX, r13: 0x%.16llX, r14: 0x%.16llX, r15: 0x%.16llX\n", frame.r12, frame.r13, frame.r14, frame.r15);
    dprintf("[KERNEL PANIC] rfl: 0x%.16llX, rip: 0x%.16llX, cs:  0x%.16llX, ss:  0x%.16llX\n", frame.rflags, frame.rip, frame.cs, frame.ss);

    // visualize_pmm(0, 0, framebuffer->width, (bitmap_size * PAGE_SIZE) / framebuffer->width);

    beep();
    hcf();
}