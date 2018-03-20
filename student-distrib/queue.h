#ifndef RTC_H
#define RTC_H

#include "type.h"

#define QUEUE_SIZE     50

volatile unsigned int rtcFront;
volatile unsigned int rtcBack;
volatile int rtcQueue[QUEUE_SIZE];

void rtcQueueInit(void);

int rtcQueueEmpty(void);

void rtcQueuePush(int value);

int rtcQueuePop(void);

#endif
