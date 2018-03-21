#ifndef RTC_H
#define RTC_H

#include "types.h"

#define HIGHEST     1024

volatile unsigned int rtcFlag;

int32_t rtc_open(void);

int32_t rtc_close(void);

int32_t rtc_read(void);

int32_t rtc_write(int *buffer, int32_t bytes);

#endif
