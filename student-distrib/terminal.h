#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.h"

/*terminal open function*/
int32_t terminal_open(uint8_t *filename);

/*terminal close funciton*/
int32_t terminal_close(int32_t fd);

/*terminal read function*/
int32_t terminal_read(int32_t fd, unsigned char *buffer, int32_t bytes);

/*terminal write function*/
int32_t terminal_write(int32_t fd, unsigned char *buffer, int32_t bytes);

#endif
