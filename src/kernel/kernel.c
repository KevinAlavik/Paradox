/*

Kernel Entry - Paradox OS
-------------------------
 · Do not return anything until you want to intentionally quit (see header for
return codes)

*/

// Kernel includes
#include <filesystem/ramdisk.h>
#include <filesystem/tar.h>
#include <filesystem/vfs.h>
#include <kernel/boot.h>
#include <kernel/kernel.h>
#include <nighterm/nighterm.h>
#include <stdint.h>
#include <system/drivers/speaker.h>
#include <system/memory/heap.h>
#include <system/memory/pmm.h>
#include <system/pic/pic.h>
#include <system/pit/pit.h>
#include <system/processes/processes.h>
#include <system/utilities/utilities.h>
#include <system/wm/wm.h>

// Corelib includes
#include <kif.h>
#include <printf.h>
#include <transform.h>

int main() {
  VFS_t *vfs = init_vfs();

  if (vfs == NULL) {
    dprintf("[\e[0;31mKernel\e[0m] Failed to initialize VFS\n");
    return KERNEL_QUIT_ERROR;
  }

  mount_drive(vfs, (uint64_t)rd, "/");

  uint64_t ramdisk_id = get_drive_id_by_label(vfs, "/");
  ramdisk_t *rd_vfs = (ramdisk_t *)read_drive(vfs, ramdisk_id);

  struct File *motd = rd_get_file(rd_vfs, "ramdisk/etc/motd");

  printf("%s\n\n", motd->content);

  printf("----------------------------\n");
  printf("Ramdisk Location: 0x%016llX\n", rd_vfs->location);
  printf("Ramdisk Size: 0x%08X\n", rd_vfs->size);
  printf("Ramdisk File Count: %d\n", rd_vfs->files);
  printf("Ramdisk Actual Size: 0x%08X\n", rd_vfs->actual_size);
  printf("----------------------------\n\n");
  for (int curFile = 0; curFile < rd_vfs->files; curFile++) {
    struct File file = rd_vfs->content->files[curFile];
    printf("%s (%d): d? %s\n", file.name, file.size,
           file.isDirectory ? "Yes" : "No");
  }

  unmount_drive(vfs, ramdisk_id);

  free(rd_vfs->content);
  free(rd);

  return KERNEL_QUIT_HANG;
}
