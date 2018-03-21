#ifndef RTC_H
#define RTC_H

#include "type.h"

#define HIGHEST     1024

volatile unsigned int rtcFlag;
volatile unsigned int rtcCount = 0;

int32_t rtc_open(void);

int32_t rtc_close(void);

int32_t rtc_read(void);

int32_t rtc_write(int freqency);

#endif
