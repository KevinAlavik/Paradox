#include "tty.h"

tty* ttys[MAX_TTYS] = { NULL };

uint8_t cur_tty_id = 0;
tty* cur_tty = NULL;

VFS_t *vfs_b = NULL;
struct limine_framebuffer *framebuffer_b = NULL;

void tty_init(VFS_t *vfs, struct limine_framebuffer *framebuffer) {
    if (vfs != NULL && framebuffer != NULL) {
        vfs_b = vfs;
        framebuffer_b = framebuffer;
    }
}

void tty_flush() {
    if (cur_tty != NULL && cur_tty->context != NULL) {
        nighterm_flush(cur_tty->context, 0, 0, 0);
        nighterm_set_cursor_position(cur_tty->context, 0, 0);
    }
}

int tty_spawn(uint8_t id, char* font_path) {
    if (id >= MAX_TTYS || ttys[id] != NULL) {
        if (ttys[id] != NULL) {
            tty_switch(id);
        }
        return 0;
    }

    tty* temp = malloc(sizeof(tty));
    if (temp == NULL) {
        return -2;
    }

    struct nighterm_ctx *context = malloc(sizeof(struct nighterm_ctx));
    if (context == NULL) {
        free(temp);
        return -2;
    }

    char *font_data;
    vfs_op_status status = driver_read(vfs_b, 0x00000000, font_path, &font_data);
    if (status != STATUS_OK) {
        free(temp);
        free(context);
        return -3;
    }

    int s = nighterm_initialize(context,
                                font_data,
                                framebuffer_b->address,
                                framebuffer_b->width,
                                framebuffer_b->height,
                                framebuffer_b->pitch,
                                framebuffer_b->bpp,
                                malloc, free);

    free(font_data);

    if (s != NIGHTERM_SUCCESS) {
        free(temp);
        free(context);
        return -4;
    }

    temp->id = id;
    temp->context = context;
    ttys[id] = temp;
    tty_switch(id);

    return 0;
}

int tty_destroy(uint8_t id) {
    if (id >= MAX_TTYS || ttys[id] == NULL) {
        return -1;
    }

    free(ttys[id]->context);
    free(ttys[id]);
    ttys[id] = NULL;

    if (cur_tty_id == id) {
        cur_tty_id = 0;
        cur_tty = NULL;
    }

    return 0;
}

int tty_get_cur() {
    return cur_tty_id;
}

void tty_switch(uint8_t id) {
    if (id >= MAX_TTYS || ttys[id] == NULL) {
        return;
    }

    cur_tty = ttys[id];
    cur_tty_id = id;
    tty_flush();
    printf("Paradox 1.4.1-dev (tty%d)\n\n", cur_tty_id);
}

void tty_write(char ch) {
    if (cur_tty != NULL && cur_tty->context != NULL) {
        nighterm_write(cur_tty->context, ch);
    }
}
