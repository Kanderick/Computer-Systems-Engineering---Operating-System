#ifndef RTC_H
#define RTC_H

#include "types.h"

#define HIGHEST     1024

volatile unsigned int rtcFlag;

int32_t rtc_open(uint8_t *filename);

int32_t rtc_close(int32_t fd);

int32_t rtc_read(int32_t fd, unsigned char *buffer, int32_t bytes);

int32_t rtc_write(int32_t fd, unsigned char *buffer, int32_t bytes);

#endif
