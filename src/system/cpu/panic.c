#include <stdint.h>
#include <system/cpu/panic.h>
#include <kernel/boot.h>
#include <nighterm/backends/vga.h>
#include <nighterm/nighterm.h>
#include <system/drivers/speaker.h>

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
    printf("rax: 0x%.16llx, rbx: 0x%.16llx, rcx: 0x%.16llx, rdx: 0x%.16llx\n", frame.rax, frame.rbx, frame.rcx, frame.rdx);
    term.cx = 4;
    printf("rsp: 0x%.16llx, rbp: 0x%.16llx, rsi: 0x%.16llx, rdi: 0x%.16llx\n", frame.rsp, frame.rbp, frame.rsi, frame.rdi);
    term.cx = 4;
    printf("r8:  0x%.16llx, r9:  0x%.16llx, r10: 0x%.16llx, r11: 0x%.16llx\n", frame.r8, frame.r9, frame.r10, frame.r11);
    term.cx = 4;
    printf("r12: 0x%.16llx, r13: 0x%.16llx, r14: 0x%.16llx, r15: 0x%.16llx\n", frame.r12, frame.r13, frame.r14, frame.r15);
    term.cx = 4;
    printf("rfl: 0x%.16llx, rip: 0x%.16llx, cs:  0x%.16llx, ss:  0x%.16llx\n", frame.rflags, frame.rip, frame.cs, frame.ss);
    term.cx = 4;

    dprintf("[KERNEL PANIC] rax: 0x%.16llx, rbx: 0x%.16llx, rcx: 0x%.16llx, rdx: 0x%.16llx\n", frame.rax, frame.rbx, frame.rcx, frame.rdx);
    dprintf("[KERNEL PANIC] rsp: 0x%.16llx, rbp: 0x%.16llx, rsi: 0x%.16llx, rdi: 0x%.16llx\n", frame.rsp, frame.rbp, frame.rsi, frame.rdi);
    dprintf("[KERNEL PANIC] r8:  0x%.16llx, r9:  0x%.16llx, r10: 0x%.16llx, r11: 0x%.16llx\n", frame.r8, frame.r9, frame.r10, frame.r11);
    dprintf("[KERNEL PANIC] r12: 0x%.16llx, r13: 0x%.16llx, r14: 0x%.16llx, r15: 0x%.16llx\n", frame.r12, frame.r13, frame.r14, frame.r15);
    dprintf("[KERNEL PANIC] rfl: 0x%.16llx, rip: 0x%.16llx, cs:  0x%.16llx, ss:  0x%.16llx\n", frame.rflags, frame.rip, frame.cs, frame.ss);

    beep();
    hcf();
}