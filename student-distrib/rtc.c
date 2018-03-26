#include "rtc.h"
#include "device.h"
#include "i8259.h"
#include "file_system.h"
#define CHECK_FAIL       -1          /* this is the open/close-failed code indicates a failure on open a file */
#define RTC_NAME_LEN           4     /* rtc file name length  */
#define RTC_CLOSED   67              /* 'C' indecates file closed */
#define RTC_OPENED   79              /* 'O' indicates file opened */
// uint8_t RTC_STATUS;

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
    int32_t ii = 0; // check input filename's string
    const uint8_t rtc_fname[RTC_NAME_LEN] = "rtc";
    /* check file name, give warnings on mis-spelling */
    if (filename == NULL)
        printf("ece391_WARNING::rtc_open has invalid filename, we implement this warning :)");
    else
        while(ii < RTC_NAME_LEN){
            if (filename[ii]!=rtc_fname[ii]){
                printf("ece391_WARNING::rtc_open has invalid filename, we implement this warning :)");
                break;
            }
            ii++;
        }
    /* check if it is already opened */
    if (file_open(rtc_fname) == CHECK_FAIL ){    /* try to open the rtc file */
        /* handle the error and return on failure */
        printf("rtc file is failed to open.\n");
        return -1;
    }
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
    /* calculate the correct fd */
    int32_t c_fd = file_find((uint8_t *) "rtc");
    /* check if the rtc file is opened */
    if(c_fd == -1){
        printf("rtc file is not opened yet, failed to close. \n");
        return -1;
    }
    /* check if the input fd is correct. Give warning on conflict. */
    else if(fd != c_fd){
        printf("ece391_WARNING::Input fd is not correct. The correct one is %d, we implement this warning :)\n", c_fd);
    }
    /* safe to close rtc now */
    return file_close(c_fd);
}

/*
 * rtc_read
 *   DESCRIPTION: rtc read function, wait until a rtc interrupt
                  has completed
 *   INPUTS: fd -- file discriptor number for the flie array
             buffer -- the buffer contains the freqency
             bytes -- the number of bytes that need to be read in
 *   OUTPUTS: none
 *   RETURN VALUE: -1 on failure, 0 on success
 *   SIDE EFFECTS: wait until a rtc interrupt has completed
 */
int32_t rtc_read(int32_t fd, unsigned char *buf, int32_t nbytes) {
    /*if file not opened, return -1*/
    if (fileStatusArray.RTC_STATUS == STATUS_CLOSED) {
      printf("Error: File not opened yet.\n");
      return -1;
    }

    /*check valid fd*/
    if (fd < 0 || fd > MAX_FILE_OPEN) {
      printf("ece391_WARNING::fd out of range.\n");
    }
    else if(fileStatusArray.FILE_TO_OPEN[fd].filetype != 0) {
      printf("ece391_WARNING::fd provided does not match an rtc file.\n");
    }

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

    /*if file not opened, return -1*/
    if (fileStatusArray.RTC_STATUS == STATUS_CLOSED) {
      printf("Error: File not opened yet.\n");
      return -1;
    }

    /*check valid fd*/
    if (fd < 0 || fd > MAX_FILE_OPEN) {
      printf("ece391_WARNING::fd out of range.\n");
    }
    else if(fileStatusArray.FILE_TO_OPEN[fd].filetype != RTC_FILE_TYPE) {
      printf("ece391_WARNING::fd provided does not match an rtc file.\n");
    }

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
    outb((prev & RTC_WRITE_MASK) | rate, RTC_REG_DATA);     /*write only our rate to A. Note, rate is the bottom 4 bits*/
    return nbytes;
}
