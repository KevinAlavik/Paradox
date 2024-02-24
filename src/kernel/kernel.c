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

  mount_drive(vfs, 0x00000000, (uint64_t)rd, "/", DISK_TYPE_RAMDISK);

  uint64_t ramdisk_id = get_drive_id_by_label(vfs, "/");

  char *buf;

  vfs_op_status status = driver_read(vfs, ramdisk_id, "/etc/motd", &buf);

  if (status != STATUS_OK) {
    dprintf("[\e[0;31mKernel\e[0m] Error reading file from disk\n");
    unmount_drive(vfs, ramdisk_id);
    free(buf);
    return KERNEL_QUIT_ERROR;
  }

  printf("%s\n", buf);

  free(buf);

  unmount_drive(vfs, ramdisk_id);
  free(vfs->drives);
  free(vfs);

  return KERNEL_QUIT_HANG;
}