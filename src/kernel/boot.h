#ifndef __BOOT_H__
#define __BOOT_H__

#include <filesystem/ramdisk.h>
#include <filesystem/vfs.h>
#include <limine.h>
#include <system/idt/idt.h>

extern volatile struct limine_module_request mod_request;
extern volatile struct limine_framebuffer_request framebuffer_request;
extern volatile struct limine_hhdm_request hhdm_request;
extern struct limine_framebuffer *framebuffer;
extern uint64_t hhdm_offset;
extern int_frame_t *cur_frame;
extern ramdisk_t *rd;
extern VFS_t *vfs;

void init_boot(int debug_info);

#endif // __BOOT_H__
