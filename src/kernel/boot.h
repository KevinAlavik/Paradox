#ifndef __BOOT_H__
#define __BOOT_H__

#include <filesystem/ramdisk.h>
#include <limine.h>

extern struct Ramdisk *ramdisk;
extern volatile struct limine_module_request mod_request;
extern volatile struct limine_framebuffer_request framebuffer_request;
extern struct limine_framebuffer *framebuffer;
void init_boot(int debug_info);

#endif // __BOOT_H__
