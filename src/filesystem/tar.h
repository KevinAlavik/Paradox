#ifndef TAR_H_
#define TAR_H_

#include <stdint.h>
#include <stddef.h>

struct tar_header
{
    char filename[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag[1];
};

extern struct tar_header *headers[32];

unsigned int getsize(const char *in);
unsigned int parse(unsigned int address);

#endif // TAR_H_
