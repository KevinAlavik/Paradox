#include "transform.h"

unsigned long long bytes_to_kilobytes(unsigned long long bytes) {
    return (bytes + 1023) / 1024;
}

unsigned long long bytes_to_megabytes(unsigned long long bytes) {
    return (bytes + (1024ULL * 1024 - 1)) / (1024ULL * 1024);
}

unsigned long long bytes_to_gigabytes(unsigned long long bytes) {
    return (bytes + (1024ULL * 1024 * 1024 - 1)) / (1024ULL * 1024 * 1024);
}

unsigned long long bytes_to_terabytes(unsigned long long bytes) {
    return (bytes + (1024ULL * 1024 * 1024 * 1024 - 1)) / (1024ULL * 1024 * 1024 * 1024);
}
