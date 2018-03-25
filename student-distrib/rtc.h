#ifndef RTC_H
#define RTC_H

#include "types.h"

#define HIGHEST         1024
#define RTC_WRITE_MASK  0xF0

volatile unsigned int rtcFlag;      /*check whether rtc is active*/

/*rtc open function*/
int32_t rtc_open(const uint8_t *filename);

/*rtc close funciton*/
int32_t rtc_close(int32_t fd);

/*rtc read function*/
int32_t rtc_read(int32_t fd, unsigned char *buf, int32_t nbytes);

/*rtc write function*/
int32_t rtc_write(int32_t fd, const unsigned char *buf, int32_t nbytes);

#endif
