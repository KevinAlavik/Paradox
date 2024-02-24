#ifndef __VFS_H__
#define __VFS_H__

#include <stddef.h>
#include <stdint.h>

#define MAX_DRIVES 256

#define DISK_TYPE_RAMDISK 0

typedef enum {
  STATUS_OK = 0,
  STATUS_ERROR_FILE_NOT_FOUND,
  STATUS_ERROR_NO_SPACE_LEFT,
  STATUS_DRIVE_NOT_FOUND,
  STATUS_MALLOC_FAILURE,
  STATUS_INVALID_ARGUMENTS,
  STATUS_INVALID_DRIVE_TYPE,
  STATUS_UNKNOWN_ERROR
} vfs_op_status;

typedef struct {
  struct VFS_t *disk;
  char *label;
} mount_point_t;

typedef struct {
  uint64_t address;
  uint64_t id;
  uint8_t type;
  mount_point_t mount_point;

  uint64_t size;
  uint64_t actual_size;
} drive_t;

typedef struct VFS_t {
  drive_t *drives;

  uint64_t address;
  uint64_t size;
  uint64_t actual_size;
} VFS_t;

VFS_t *init_vfs();

int mount_drive(VFS_t *vfs, uint64_t id, uint64_t address, char *label,
                uint8_t type);
int unmount_drive(VFS_t *disk, uint64_t id);

vfs_op_status driver_read(VFS_t *disk, uint64_t id, const char *filepath,
                          char **buf);
vfs_op_status driver_write(VFS_t *disk, uint64_t id, const char *filepath,
                           const char *buf);

uint64_t get_drive_id_by_label(VFS_t *vfs, const char *label);

#endif // __VFS_H__