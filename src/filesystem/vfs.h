#ifndef __VFS_H__
#define __VFS_H__

// VFS Driver for Paradox

#include <stdint.h>

typedef struct {
  struct VFS_t *disk;
  char *label;
} mount_point_t;

typedef struct {
  uint64_t address;
  uint64_t id;
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

int mount_drive(VFS_t *vfs, uint64_t address, char *label);
int unmount_drive(VFS_t *disk, uint64_t id);

void *read_drive(VFS_t *disk, uint64_t id);
void write_drive(VFS_t *disk, uint64_t id, void *data);

uint64_t get_drive_id_by_label(VFS_t *vfs, const char *label);

#endif // __VFS_H__
