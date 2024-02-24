#include "vfs.h"

#include <filesystem/ramdisk.h>

#include <stddef.h>
#include <stdlib.h>
#include <strings.h>

VFS_t *init_vfs() {
  VFS_t *vfs = (VFS_t *)malloc(sizeof(VFS_t));
  if (vfs == NULL) {
    return NULL;
  }

  vfs->drives = (drive_t *)malloc(MAX_DRIVES * sizeof(drive_t));
  if (vfs->drives == NULL) {
    free(vfs);
    return NULL;
  }

  vfs->address = (uint64_t)vfs;
  vfs->size = sizeof(VFS_t);
  vfs->actual_size = 0;

  dprintf("[\e[0;32mVFS\e[0m] Initialized VFS at 0x%08llX\n", vfs->address);

  return vfs;
}

int mount_drive(VFS_t *vfs, uint64_t id, uint64_t address, char *label,
                uint8_t type) {
  if (vfs == NULL) {
    dprintf("[\e[0;31mVFS\e[0m] NULL VFS passed!\n");
    return -1;
  }

  if (id < 0 || id >= MAX_DRIVES) {
    dprintf("[\e[0;31mVFS\e[0m] Invalid drive id!\n");
    return -1;
  }

  if (address == 0) {
    dprintf("[\e[0;31mVFS\e[0m] Invalid address passed!\n");
    return -1;
  }

  drive_t temp_drive;
  temp_drive.address = address;
  temp_drive.size = sizeof(drive_t);
  temp_drive.actual_size = sizeof(drive_t);
  temp_drive.type = type;

  mount_point_t temp_mount_point;
  temp_mount_point.disk = vfs;
  temp_mount_point.label = label;

  temp_drive.mount_point = temp_mount_point;

  temp_drive.id = id;

  vfs->drives[id] = temp_drive;

  dprintf(
      "[\e[0;32mVFS\e[0m] Mounted drive with id 0x%08llX from 0x%08llX\n\e[0m",
      temp_drive.id, temp_drive.address);

  return 0;
}

int unmount_drive(VFS_t *vfs, uint64_t id) {
  if (vfs == NULL) {
    dprintf("[\e[0;31mVFS\e[0m] NULL VFS passed for unmounting!\n");
    return -1;
  }

  if (id < 0 || id >= MAX_DRIVES) {
    dprintf("[\e[0;31mVFS\e[0m] Invalid drive id 0x%08llX\n", id);
    return -1;
  }

  vfs->drives[id] = (drive_t){0};

  dprintf("[\e[0;32mVFS\e[0m] Unmounted drive with id 0x%08llX\n", id);

  return 0;
}

vfs_op_status driver_read(VFS_t *disk, uint64_t id, const char *filepath,
                          char **buf) {
  if (disk == NULL || buf == NULL) {
    dprintf(
        "[\e[0;31mVFS\e[0m] NULL VFS or buffer passed for reading drive!\n");
    return STATUS_INVALID_ARGUMENTS;
  }

  drive_t *drive = &disk->drives[id];

  if (drive == NULL) {
    dprintf("[\e[0;31mVFS\e[0m] Drive with id 0x%08llX not found!\n", id);
    return STATUS_DRIVE_NOT_FOUND;
  }

  switch (drive->type) {
  case DISK_TYPE_RAMDISK:
    ramdisk_t *temp_rd = (ramdisk_t *)drive->address;
    struct File *temp_file = rd_get_file(temp_rd, filepath);

    if (temp_file == NULL) {
      dprintf("[\e[0;31mVFS\e[0m] File not found!\n");
      return STATUS_ERROR_FILE_NOT_FOUND;
    }

    *buf = strdup(temp_file->content);
    if (*buf == NULL) {
      dprintf("[\e[0;31mVFS\e[0m] Failed to allocate memory for buffer\n");
      return STATUS_MALLOC_FAILURE;
    }

    dprintf("[\e[0;32mVFS\e[0m] Successfully read from drive!\n");
    break;
  default:
    dprintf("[\e[0;31mVFS\e[0m] The drive has an invalid type? (%d)\n",
            drive->type);
    return STATUS_INVALID_DRIVE_TYPE;
  }
  return STATUS_OK;
}

vfs_op_status driver_write(VFS_t *disk, uint64_t id, const char *filepath,
                           const char *buf) {

  if (disk == NULL || buf == NULL) {
    dprintf(
        "[\e[0;31mVFS\e[0m] NULL VFS or buffer passed for writing to drive!\n");
    return STATUS_INVALID_ARGUMENTS;
  }

  drive_t *drive = &disk->drives[id];

  if (drive == NULL) {
    dprintf("[\e[0;31mVFS\e[0m] Drive with id 0x%08llX not found!\n", id);
    return STATUS_DRIVE_NOT_FOUND;
  }

  switch (drive->type) {
  case DISK_TYPE_RAMDISK:
    ramdisk_t *temp_rd = (ramdisk_t *)drive->address;
    struct File *temp_file = rd_get_file(temp_rd, filepath);

    if (temp_file == NULL) {
      dprintf("[\e[0;31mVFS\e[0m] File not found!\n");
      return STATUS_ERROR_FILE_NOT_FOUND;
    }

    free(temp_file->content);
    temp_file->content = strdup(buf);

    dprintf("[\e[0;32mVFS\e[0m] Successfully wrote to drive!\n");
    break;
  default:
    dprintf("[\e[0;31mVFS\e[0m] The drive has an invalid type? (%d)\n",
            drive->type);
    return STATUS_INVALID_DRIVE_TYPE;
  }
  return STATUS_OK;
}

uint64_t get_drive_id_by_label(VFS_t *vfs, const char *label) {
  if (vfs == NULL || label == NULL) {
    dprintf(
        "[\e[0;31mVFS\e[0m] NULL VFS or label passed for getting drive id!\n");
    return -1;
  }

  for (int i = 0; i < MAX_DRIVES; i++) {
    if (vfs->drives[i].mount_point.label != NULL &&
        strcmp(vfs->drives[i].mount_point.label, label) == 0) {
      dprintf("[\e[0;32mVFS\e[0m] Found drive at \"%s\" with id 0x%08llX\n",
              label, vfs->drives[i].id);
      return vfs->drives[i].id;
    }
  }

  dprintf("[\e[0;31mVFS\e[0m] Drive with label \"%s\" not found!\n", label);
  return -1;
}

size_t vfs_get_file_size(VFS_t *vfs, uint64_t id, const char *filepath) {
  if (vfs == NULL || filepath == NULL) {
    dprintf("[\e[0;31mVFS\e[0m] Invalid arguments: vfs or filepath is NULL\n");
    return 0;
  }

  drive_t *drive = &vfs->drives[id];
  if (drive == NULL) {
    dprintf("[\e[0;31mVFS\e[0m] Invalid drive: id %llu\n", id);
    return 0;
  }

  switch (drive->type) {
  case DISK_TYPE_RAMDISK: {
    ramdisk_t *rd = (ramdisk_t *)(uintptr_t)(drive->address);
    if (rd == NULL) {
      dprintf("[\e[0;31mVFS\e[0m] RAM disk not initialized\n");
      return 0;
    }

    for (unsigned int i = 0; i < rd->files; i++) {
      if (strcmp(rd->content->files[i].name, filepath) == 0) {
        dprintf("[\e[0;32mVFS\e[0m] File found: %s, Size: %u bytes\n", filepath,
                rd->content->files[i].size);
        return rd->content->files[i].size;
      }
    }
    dprintf("[\e[0;31mVFS\e[0m] File not found: %s\n", filepath);
    break;
  }
  default:
    dprintf("[\e[0;31mVFS\e[0m] Unsupported drive type: %d\n", drive->type);
    return 0;
  }

  return 0;
}
