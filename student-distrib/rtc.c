#include "rtc.h"
#include "queue.h"
#include "device.h"
#include "i8259.h"

int rtc_open() {
    rtc_write(2);
    enable_irq(RTC_IRQ);
    return 0;
}

int rtc_close() {
    return 0;
}

int rtc_read() {
    sti();
    rtcFlag = 1;
    while (rtcFlag);
    return 0;
}

int rtc_write(int freqency) {
    char prev;
    int curFreqency;
    rtcQueuePush(freqency);
    if (rtcQueueEmpty()) return -1;
    curFreqency = rtcQueuePop();
    int rate = MAX_RATE;
    if (curFreqency > HIGHEST || curFreqency <= 1) return -1;
    if (curFreqency & (curFreqency - 1)) {
        printf("input is not power of 2");
        return -1;
    }
    while (curFreqency != 1) {
        curFreqency /= 2;
        rate --;
    }
    outb(SR_A, RTC_REG_NUM);      /*set index to register A, disable NMI*/
    prev = inb(RTC_REG_DATA);         /*get initial value of register A*/
    outb(SR_A, RTC_REG_NUM);      /*reset index to A*/
    outb((prev & 0xF0) | rate, RTC_REG_DATA);     /*write only our rate to A. Note, rate is the bottom 4 bits*/
    return 0;
}
