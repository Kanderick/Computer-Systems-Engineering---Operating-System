#ifndef RTC_H
#define RTC_H

#include "type.h"

#define HIGHEST     1024

volatile unsigned int rtcFlag;
volatile unsigned int rtcCount = 0;

int rtc_open(void);

int rtc_close(void);

int rtc_read(void);

int rtc_write(int freqency);

#endif
