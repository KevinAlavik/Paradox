#ifndef __TGA_H__
#define __TGA_H__

#ifdef __cplusplus
extern "C" {
#endif

unsigned int *tga_parse(unsigned char *ptr, int size);

void draw_targa_image(char *image, int size, int x, int y);

#ifdef __cplusplus
}
#endif

#endif /* __TGA_H__ */