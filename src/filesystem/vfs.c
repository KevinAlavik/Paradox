#include "vfs.h"
#include <stddef.h>
#include <stdlib.h>
#include <strings.h>

VFS_t *init_vfs() {
  VFS_t *vfs = (VFS_t *)malloc(sizeof(VFS_t));
  if (vfs == NULL) {
    return NULL;
  }

  vfs->drives = NULL;
  vfs->address = (uint64_t)vfs;
  vfs->size = sizeof(VFS_t) * sizeof(vfs);
  vfs->actual_size = 0;

  dprintf("[\e[0;32mVFS\e[0m] Initialized VFS at 0x%08llX\n", vfs->address);

  return vfs;
}

int mount_drive(VFS_t *vfs, uint64_t address, char *label) {
  if (vfs == NULL) {
    dprintf("[\e[0;31mVFS\e[0m] NULL VFS passed!\n");
    return -1;
  }

  if (address == 0) {
    dprintf("[\e[0;31mVFS\e[0m] Invalid address passed!\n");
    return -1;
  }

  if (vfs->drives == NULL) {
    vfs->drives = (drive_t *)malloc(sizeof(drive_t));
    if (vfs->drives == NULL) {
      dprintf("[\e[0;31mVFS\e[0m] Memory malloc failed!\n");
      return -1;
    }
  } else {
    drive_t *temp = (drive_t *)realloc(vfs->drives, (vfs->actual_size + 1) *
                                                        sizeof(drive_t));
    if (temp == NULL) {
      dprintf("[\e[0;31mVFS\e[0m] Memory realloc failed!\n");
      return -1;
    }
    vfs->drives = temp;
  }

  drive_t temp_drive;
  temp_drive.address = address;
  temp_drive.size = sizeof(drive_t);
  temp_drive.actual_size = sizeof(drive_t);

  mount_point_t temp_mount_point;
  temp_mount_point.disk = vfs;
  temp_mount_point.label = label;

  temp_drive.mount_point = temp_mount_point;

  int i;
  for (i = vfs->actual_size - 1; i >= 0; i--) {
    if (vfs->drives[i].address != 0) {
      break;
    }
  }
  temp_drive.id = i + 1; // Assigning ID based on last non-null drive

  vfs->drives[vfs->actual_size] = temp_drive;
  vfs->actual_size++;

  dprintf(
      "[\e[0;32mVFS\e[0m] Mounted drive with id 0x%08llX at 0x%08llX\n\e[0m",
      vfs->drives[vfs->actual_size - 1].id,
      vfs->drives[vfs->actual_size - 1].address);

  return 0;
}

int unmount_drive(VFS_t *vfs, uint64_t id) {
  if (vfs == NULL) {
    dprintf("[\e[0;31mVFS\e[0m] NULL VFS passed for unmounting!\n");
    return -1;
  }

  if (vfs->drives == NULL) {
    dprintf("[\e[0;31mVFS\e[0m] No drives mounted to unmount!\n");
    return -1;
  }

  if (id < 0 || id >= vfs->actual_size) {
    dprintf("[\e[0;31mVFS\e[0m] Invalid drive id 0x%08llX\n", id);
    return -1;
  }

  vfs->drives[id] = (drive_t){0}; // Clearing the drive data
  vfs->actual_size--;

  dprintf("[\e[0;32mVFS\e[0m] Unmounted drive with id 0x%08llX\n", id);

  return 0;
}

void write_drive(VFS_t *vfs, uint64_t id, void *data) {
  if (vfs == NULL) {
    dprintf("[\e[0;31mVFS\e[0m] NULL VFS passed for writing drive!\n");
    return;
  }

  if (id < 0 || id >= vfs->actual_size) {
    dprintf("[\e[0;31mVFS\e[0m] Invalid drive id 0x%08llX\n", id);
    return;
  }

  vfs->drives[id].address = (uint64_t)data;
  dprintf("[\e[0;32mVFS\e[0m] Wrote data to drive with id 0x%08llX at "
          "address 0x%08llX\n",
          id, vfs->drives[id].address);
}

void *read_drive(VFS_t *vfs, uint64_t id) {
  if (vfs == NULL) {
    dprintf("[\e[0;31mVFS\e[0m] NULL VFS passed for reading drive!\n");
    return NULL;
  }

  if (id < 0 || id >= vfs->actual_size) {
    dprintf("[\e[0;31mVFS\e[0m] Invalid drive id 0x%08llX\n", id);
    return NULL;
  }

  // Accessing the correct drive using the provided ID
  dprintf("[\e[0;32mVFS\e[0m] Read data from drive with id 0x%08llX at address "
          "0x%08llX\n",
          id, vfs->drives[id].address);
  return (void *)vfs->drives[id].address;
}

uint64_t get_drive_id_by_label(VFS_t *vfs, const char *label) {
  if (vfs == NULL || label == NULL) {
    dprintf(
        "[\e[0;31mVFS\e[0m] NULL VFS or label passed for getting drive id!\n");
    return -1;
  }

  for (int i = 0; i < vfs->actual_size; i++) {
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
