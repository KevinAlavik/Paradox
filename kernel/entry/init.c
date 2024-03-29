#include <entry/init.h>
#include <entry/kernel.h>
#include <nighterm/nighterm.h>
#include <printf.h>
#include <x86_64/cpu/cpu.h>
#include <x86_64/cpu/panic.h>
#include <system/devices/keyboard.h>
#include <x86_64/idt/idt.h>
#include <system/memory/pmm.h>
#include <system/pic/pic.h>
#include <system/pit/pit.h>
#include <utilities/utilities.h>
#include <system/pci/pci.h>

volatile struct limine_module_request mod_request = {
    .id = LIMINE_MODULE_REQUEST, .revision = 0};

volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 1};

volatile struct limine_hhdm_request hhdm_request = {.id = LIMINE_HHDM_REQUEST,
                                                    .revision = 0};

struct limine_framebuffer *framebuffer;
uint64_t hhdm_offset;
int_frame_t *cur_frame;
ramdisk_t *rd;
VFS_t *vfs;

void init()
{
  hhdm_offset = hhdm_request.response->offset;
  framebuffer = framebuffer_request.response->framebuffers[0];

  dprintf("[\e[0;32mSystem\e[0m] Initialized kmsg stream.\n");
  init_idt();
  dprintf("[\e[0;32mSystem\e[0m] Initialized IDT (And IRQ)\n");
  init_pmm();
  dprintf("[\e[0;32mSystem\e[0m] Initialized PMM\n");
  pit_init();
  dprintf("[\e[0;32mSystem\e[0m] Initialized PIT\n");
  mouse_init();
  dprintf("[\e[0;32mSystem\e[0m] Initialized Mouse\n");
  init_keyboard();
  dprintf("[\e[0;32mSystem\e[0m] Initialized Keyboard\n");
  rd = init_rd();

  if (rd == NULL)
  {
    dprintf("[\e[0;32mRamdisk\e[0m] Failed to initialize ramdisk\n");
    return;
  }

  vfs = init_vfs();
  if (vfs == NULL)
  {
    dprintf("[\e[0;31mVFS\e[0m] Failed to initialize VFS\n");
    return;
  }

  mount_drive(vfs, 0x00000000, (uint64_t)rd, "/", DISK_TYPE_RAMDISK);

  dprintf("[\e[0;32mVFS\e[0m] Mounted ramdisk\n");

  tty_init(vfs, framebuffer);

  keyboard.out = false;
  tty_spawn(0, FONT_SMALL);
  register_pci();

  int kstatus = kmain(); // Launch the kernel

  if (kstatus == KERNEL_QUIT_SUCCESS)
  {
    dprintf(
        "[\e[0;32mKernel\e[0m Success] Kernel quit successfully, shutting down "
        "in 10 "
        "seconds!\n");
    pit_sleep(10000);
    shutdown();
  }
  else if (kstatus == KERNEL_QUIT_ERROR)
  {
    dprintf(
        "[\e[0;32mKernel Error\e[0m] A kernel error occured, check kmesg for reason! "
        "Rebooting...\n");
    reboot();
  }
  else if (kstatus == KERNEL_QUIT_PANIC)
  {
    panic("[\e[0;32mKernel Panic\e[0m] Kernel quit with a critical error code, please see "
          "kmsg for extra info.\n",
          *cur_frame);
    dprintf(
        "[\e[0;32mKernel Panic\e[0m] Kernel quit with a critical error code, "
        "the kernel "
        "process returned with a 2 that means something went really wrong, "
        "and we are shutting down your computer in 10 seconds!\n");
    pit_sleep(10000);
    shutdown();
  }
  else if (kstatus == KERNEL_QUIT_HANG)
  {
    hlt();
  }
  else
  {
    dprintf("[\e[0;32mKernel Warning\e[0m] Kernel returned %d\n", kstatus);
  }

  // If we reach this point, something unexpected happened
  dprintf("[\e[0;32mError] Unexpected behavior occurred, shutting down...\n");
  shutdown();
}
