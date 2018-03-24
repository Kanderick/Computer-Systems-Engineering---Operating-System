#include "rtc.h"
#include "device.h"
#include "i8259.h"

/*
 * rtc_open
 *   DESCRIPTION: rtc open function, set the initial rate to 2Hz
                  and open the rtc irq
 *   INPUTS: filename -- the file name of the rtc
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: set the initial rate to 2Hz and open the rtc irq
 */
int32_t rtc_open(const uint8_t *filename) {
    set_rate(RATE);     /*set the rate to 2Hz*/
    enable_irq(RTC_IRQ);
    return 0;
}

/*
 * rtc_close
 *   DESCRIPTION: rtc close function
 *   INPUTS: fd -- file discriptor number for the flie array
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: close the rtc file
 */
int32_t rtc_close(int32_t fd) {
    return 0;
}

/*
 * rtc_read
 *   DESCRIPTION: rtc read function, wait until a rtc interrupt
                  has completed
 *   INPUTS: fd -- file discriptor number for the flie array
             buffer -- the buffer contains the freqency
             bytes -- the number of bytes that need to be read in
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: wait until a rtc interrupt has completed
 */
int32_t rtc_read(int32_t fd, unsigned char *buf, int32_t nbytes) {
    sti();
    rtcFlag = 1;    /*set the rtc flag to 1*/
    while (rtcFlag);    /*check whether a rtc interrupt completed*/
    return 0;
}

/*
 * rtc_write
 *   DESCRIPTION: rtc write function, set the rtc freqency to the
                  input one
 *   INPUTS: fd -- file discriptor number for the flie array
             buffer -- the buffer contains the freqency
             bytes -- the number of bytes that need to be read in
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: set the rtc freqency to the input one
 */
int32_t rtc_write(int32_t fd, const unsigned char *buf, int32_t nbytes) {
    char prev;
    int32_t freqency;
    if (buf == NULL) return -1;      /*check whether the buffer is valid*/
    if (nbytes != 4) return -1;      /*the freqency is a 4 bytes int*/
    freqency = *buf;
    int rate = MAX_RATE;
    if (freqency > HIGHEST || freqency <= 1) return -1;     /*freqency is out of range*/
    if (freqency & (freqency - 1)) {        /*input is not power of 2*/
        printf("input is not power of 2");
        return -1;
    }
    while (freqency != 1) {         /*convert the freqency to rate*/
        freqency /= 2;
        rate --;
    }
    outb(SR_A, RTC_REG_NUM);      /*set index to register A, disable NMI*/
    prev = inb(RTC_REG_DATA);         /*get initial value of register A*/
    outb(SR_A, RTC_REG_NUM);      /*reset index to A*/
    outb((prev & 0xF0) | rate, RTC_REG_DATA);     /*write only our rate to A. Note, rate is the bottom 4 bits*/
    return nbytes;
}
