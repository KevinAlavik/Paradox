#include "tty.h"

tty* ttys[MAX_TTYS];

uint8_t cur_tty_id;
tty* cur_tty;
