#include "rtc.h"
#include "device.h"
#include "i8259.h"

int32_t rtc_open() {
    set_rate(RATE);
    enable_irq(RTC_IRQ);
    return 0;
}

int32_t rtc_close() {
    return 0;
}

int32_t rtc_read() {
    sti();
    rtcFlag = 1;
    while (rtcFlag);
    return 0;
}

int32_t rtc_write(int *buffer, int32_t bytes) {
    char prev;
    int32_t freqency;
    if (buffer == NULL) return -1;
    if (bytes != 4) return -1;
    freqency = *buffer;
    int rate = MAX_RATE;
    if (freqency > HIGHEST || freqency <= 1) return -1;
    if (freqency & (freqency - 1)) {
        printf("input is not power of 2");
        return -1;
    }
    while (freqency != 1) {
        freqency /= 2;
        rate --;
    }
    outb(SR_A, RTC_REG_NUM);      /*set index to register A, disable NMI*/
    prev = inb(RTC_REG_DATA);         /*get initial value of register A*/
    outb(SR_A, RTC_REG_NUM);      /*reset index to A*/
    outb((prev & 0xF0) | rate, RTC_REG_DATA);     /*write only our rate to A. Note, rate is the bottom 4 bits*/
    return sizeof(freqency);
}
