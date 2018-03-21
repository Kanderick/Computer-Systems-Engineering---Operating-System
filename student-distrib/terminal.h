#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.h"

int32_t terminal_open(void);

int32_t terminal_close(void);

int32_t terminal_read(unsigned char *buffer, int32_t bytes);

int32_t terminal_write(unsigned char *buffer, int32_t bytes);

#endif
