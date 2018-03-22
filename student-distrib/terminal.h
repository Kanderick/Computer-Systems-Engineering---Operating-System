#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.h"

int32_t terminal_open(uint8_t *filename);

int32_t terminal_close(int32_t fd);

int32_t terminal_read(int32_t fd, unsigned char *buffer, int32_t bytes);

int32_t terminal_write(int32_t fd, unsigned char *buffer, int32_t bytes);

#endif
