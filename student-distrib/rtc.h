#ifndef RTC_H
#define RTC_H

#include "types.h"

#define HIGHEST     1024

volatile unsigned int rtcFlag;      /*check whether rtc is active*/

/*rtc open function*/
int32_t rtc_open(uint8_t *filename);

/*rtc close funciton*/
int32_t rtc_close(int32_t fd);

/*rtc read function*/
int32_t rtc_read(int32_t fd, unsigned char *buffer, int32_t bytes);

/*rtc write function*/
int32_t rtc_write(int32_t fd, unsigned char *buffer, int32_t bytes);

#endif
