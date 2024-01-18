#include <kernel/boot.h>
#include <limine.h>
#include <nighterm/nighterm.h>
#include <printf.h>
#include <system/cpu/cpu.h>
#include <system/idt/idt.h>
#include <system/logger/sys_log.h>
#include <system/memory/memory.h>
#include <system/utilities/utilities.h>
#include <filesystem/ramdisk.h>

static volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST, .revision = 0};

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 1};

void init_boot(int debug_info) {
  static struct limine_framebuffer *framebuffer;

  struct limine_file *tarfile = mod_request.response->modules[0];
  framebuffer = framebuffer_request.response->framebuffers[0];

  int rstatus = init_ramdisk(tarfile);
  if (!rstatus) {
    dprintf("Ramdisk failed to initialize, got status: %d", rstatus);
    hcf();
  }

  int nstatus = nighterm_initialize(ramdisk->tar->files[0].content, framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch, framebuffer->bpp, malloc);
  // TODO: Dont hard code the `nighterm_initialize` to use `ramdisk->tar->files[0].content` as a font instead search for the font. In this case i know that the font is the only thing in the ramdisk so.
  
  if (!nstatus) {
    dprintf("Nighterm failed to initialize, got status: %s", get_nighterm_return_string(nstatus));
    hcf();
  }
  log(OK, "Initiaized Nighterm");
  init_idt();
  log(OK, "Initiaized IDT");
  init_physical_memory();
  log(OK, "Initiaized PMM");
  // nighterm_clear();
  printf("\n");
  printf("Welcome to ");
  printfColor(77, 166, 255, 0, 0, 0, "Paradox\n");
  char *nreturn = get_nighterm_return_string(nstatus);

  if (debug_info) {
    printf(
        "Debug Info:\n - %hhu module(s) loaded\n - Nighterm return code: %s\n",
        mod_request.response->module_count, nreturn);

    for (unsigned int i = 0; i < ramdisk->fileCount; ++i) {
      printf(" - %s", ramdisk->tar->files[i].name);
    }
  }

  for (int i = 0; i < mod_request.response->module_count; i++) {
    printf("%s: %u\n", mod_request.response->modules[i]->path,
           mod_request.response->modules[i]->size);
  }
}
