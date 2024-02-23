#ifndef __VFS_H__
#define __VFS_H__

// VFS Driver for Paradox

#include <stdint.h>

typedef struct {
  VFS_t* disk;
  char* label;
} mount_point_t;

typedef struct {
  uint64_t address;
  uint64_t id;
  mount_point_t mount_point;
  
  uint64_t size;
  uint64_t actual_size;
} drive_t;

typedef struct {
  drive_t* drives;

  uint64_t address;
  uint64_t size;
  uint64_t actual_size;
} VFS_t;

VFS_t* init_vfs();

int mount_drive(VSF_t* disk, drive_t drive);
int unmount_drive(VSF_t* disk, uint64_t id);

void* read_drive(VSF_t* disk, uint64_t id);
void write_drive(VSF_t* disk, uint64_t id, void* data);

#endif // __VFS_H_
