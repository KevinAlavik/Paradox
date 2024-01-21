#include <filesystem/ramdisk.h>
#include <kernel/boot.h>
#include <limine.h>
#include <nighterm/nighterm.h>
#include <printf.h>
#include <system/cpu/cpu.h>
#include <system/idt/idt.h>
#include <system/logger/sys_log.h>
#include <system/memory/memory.h>
#include <system/utilities/utilities.h>

static volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST, .revision = 0};

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 1};

void init_boot(int debug_info) {
  static struct limine_framebuffer *framebuffer;

  framebuffer = framebuffer_request.response->framebuffers[0];
  struct limine_file *rdisk = mod_request.response->modules[0];
  dprintf("\033c");

  init_idt();
  dprintf("Initialized IDT\n");
  init_physical_memory();
  dprintf("Initialized PMM\n");

  struct Ramdisk *ramdisk;
  int rstatus = init_ramdisk(rdisk, &ramdisk);

  if (rstatus) {
    dprintf("Ramdisk failed to initialize, got status: %d\n", rstatus);
    hcf();
  }

  dprintf("Loaded ramdisk, file count: %d", ramdisk->fileCount);

  for (int i = 0; i < ramdisk->tar->fileCount; i++) {
    struct File *file = &(ramdisk->tar->files[i]);
    dprintf("- %s", file->name);
  }
}
