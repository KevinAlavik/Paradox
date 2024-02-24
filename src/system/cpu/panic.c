#include <filesystem/tar.h>
#include <kernel/boot.h>
#include <kif.h>
#include <nighterm/nighterm.h> // Dont make this need nighterm.h to draw things on the screen. Use vga.h
#include <printf.h>
#include <stdint.h>
#include <system/cpu/panic.h>
#include <system/drivers/speaker.h>
#include <system/memory/pmm.h>
#include <vga.h>

void panic(const char *reason, int_frame_t frame) {
  flush(70, 105, 255);
  nighterm_set_bg_color(70, 105, 255);

  struct File *img = rd_get_file(rd, "/etc/images/bsod.kif");
  if (img == NULL) {
    dprintf("[\e[0;32mSystem\e[0m] Failed to load panic image! Didnt "
            "find: /etc/images/bsod.kif\n");
    return;
  }
  draw_image(img->content, 0, 0, 0);

  printf("\tComputer no workie :( \n\n");

  printf("\t* %s\n\n", reason);

  nighterm_set_fg_color(201, 183, 193);

  printf("\trax: 0x%.16llX, rbx: 0x%.16llX, rcx: 0x%.16llX, rdx: 0x%.16llX\n",
         frame.rax, frame.rbx, frame.rcx, frame.rdx);

  printf("\trsp: 0x%.16llX, rbp: 0x%.16llX, rsi: 0x%.16llX, rdi: 0x%.16llX\n",
         frame.rsp, frame.rbp, frame.rsi, frame.rdi);

  printf("\tr8:  0x%.16llX, r9:  0x%.16llX, r10: 0x%.16llX, r11: 0x%.16llX\n",
         frame.r8, frame.r9, frame.r10, frame.r11);

  printf("\tr12: 0x%.16llX, r13: 0x%.16llX, r14: 0x%.16llX, r15: 0x%.16llX\n",
         frame.r12, frame.r13, frame.r14, frame.r15);

  printf("\trfl: 0x%.16llX, rip: 0x%.16llX, cs:  0x%.16llX, ss:  0x%.16llX\n",
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