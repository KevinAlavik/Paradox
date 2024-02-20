/*

Kernel Entry - Paradox OS
-------------------------
 · Do not return anything until you want to intentionally quit (see header for
return codes)

*/

// Kernel includes
#include <filesystem/tar.h>
#include <kernel/boot.h>
#include <kernel/kernel.h>
#include <nighterm/nighterm.h>
#include <stdint.h>
#include <system/drivers/speaker.h>
#include <system/logger/sys_log.h>
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

  const char *rawData = (char *)mod_request.response->modules[2]->address;
  unsigned int dataSize = mod_request.response->modules[2]->size;

  struct Tar tar;
  extractTarData(rawData, dataSize, &tar);

  for (unsigned int i = 0; i < tar.fileCount; ++i) {
    printf("%s\n", tar.files[i].name);
    if (!tar.files[i].isDirectory) {
      printf("\t%s\n", tar.files[i].content);
    }
    printf(" * Is Directory: %s\n\n", tar.files[i].isDirectory ? "Yes" : "No");
  }

  freeTar(&tar);

  return KERNEL_QUIT_SUCCESS;
}
