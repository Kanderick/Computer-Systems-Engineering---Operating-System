#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.h"
#include "device.h"

#define TERMINAL_BUFEER_SIZE    BUFF_SIZE

/*terminal open function*/
int32_t terminal_open(const uint8_t *filename);

/*terminal close funciton*/
int32_t terminal_close(int32_t fd);

/*terminal read function*/
int32_t terminal_read(int32_t fd, unsigned char *buf, int32_t nbytes);

/*terminal write function*/
int32_t terminal_write(int32_t fd, const unsigned char *buf, int32_t nbytes);

#endif
