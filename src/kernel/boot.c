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

  struct limine_file *rdisk = mod_request.response->modules[0];
  framebuffer = framebuffer_request.response->framebuffers[0];


  int rstatus = init_ramdisk(rdisk);
  if (!rstatus) {
    dprintf("Ramdisk failed to initialize, got status: %d", rstatus);
    hcf();
  }

  int nstatus = nighterm_initialize(NULL, framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch, framebuffer->bpp, malloc);

  if (!nstatus) {
    dprintf("Nighterm failed to initialize, got status: %s",
            get_nighterm_return_string(nstatus));
    hcf();
  }


  init_idt();

  for (int i = 0; i < mod_request.response->module_count; i++) {
    printf("%s: %u\n", mod_request.response->modules[i]->path,
           mod_request.response->modules[i]->size);
  }
}
