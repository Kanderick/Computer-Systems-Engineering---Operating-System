#include "queue.h"

void rtcQueueInit() {
    rtcFront = 0;
    rtcBack = 0;
}

int rtcQueueEmpty() {
    if (front == back) return 1;
    else return 0;
}

void rtcQueuePush(int value) {
    rtcQueue[rtcBack] = value;
    rtcBack ++;
    if (rtcBack >= QUEUE_SIZE) rtcBack = 0;
}

int rtcQueuePop() {
    int popValue = rtcQueue[rtcFront];
    rtcFront ++;
    if (rtcFront >= QUEUE_SIZE) rtcFront = 0;
    return popValue;
}
