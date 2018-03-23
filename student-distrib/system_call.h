#ifndef SYSTEM_CALL_H
#define SYSTEM_CALL_H

#include "types.h"

int32_t open(const uint8_t *filename);

int32_t close(int32_t fd);

int32_t read(int32_t fd, void *buf, int32_t nbytes);

int32_t write(int32_t fd, const void *buf, int32_t nbytes);

#endif
