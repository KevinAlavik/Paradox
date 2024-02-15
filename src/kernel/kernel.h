#ifndef __KERNEL_H__
#define __KERNEL_H__

#define KERNEL_QUIT_SUCCESS 0 // Return this to quit and shutdown
#define KERNEL_QUIT_ERROR 1   // Return this to quit (on error) and retry (shutdown if failed to retry)
#define KERNEL_QUIT_PANIC 2   // Return this on kernel panic, shutdowns immediately

int main(); // ONLY RETURN TO QUIT

#endif // __KERNEL_H__