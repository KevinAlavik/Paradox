#ifndef __INIT_H__
#define __INIT_H__

#include <filesystem/ramdisk.h>
#include <filesystem/vfs.h>
#include <limine.h>
#include <x86_64/idt/idt.h>

#define FONT_SMALL "/usr/share/fonts/Uni3-Terminus12x6.psf"
#define FONT_BIG "/usr/share/fonts/Uni3-Terminus20x10.psf"

extern volatile struct limine_module_request mod_request;
extern volatile struct limine_framebuffer_request framebuffer_request;
extern volatile struct limine_hhdm_request hhdm_request;
extern struct limine_framebuffer *framebuffer;
extern uint64_t hhdm_offset;
extern int_frame_t *cur_frame;
extern ramdisk_t *rd;
extern VFS_t *vfs;

void init();

#endif // __INIT_H__
