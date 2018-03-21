/* device.c - Functions to initialize the devices and set up
their handler
 */
#include "device.h"
#include "i8259.h"
#include "tests.h"
#include "rtc.h"
#include "lib.h"

static uint8_t shiftFlag;       /* check whether the shiftkey is pressed */
static uint8_t ctrlFlag;
static uint8_t altFlag;
static uint8_t capsFlag;
static volatile uint8_t enterFlag;
static unsigned char keyBuffer[BUFF_SIZE];
static int buffIdx = 0;

/*
 * keyboard_interrupt
 *   DESCRIPTION: this function will be called by the keyboard_wrapper
                  and print the pressed key onto the screen
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the pressed key onto the screen
 */
void keyboard_interrupt() {
    cli();                          /*clean the interrupt flag*/
    send_eoi(KEYBOARD_IRQ);         /*send the end of interrupt signal to PIC*/
    sti();                          /*restore the interrupt flag*/
    unsigned char scancode = 0;     /*initialize scancode*/
    unsigned char pressedKey = 0;   /*initialize pressedKey*/
    while (!(inb(KEY_REG_STATUS) & 1));         /*check whether the first bit of status reg is set to one*/
    do {
        if (inb(KEY_REG_DATA) != scancode) {    /* check whether a key is pressed */
            scancode = inb(KEY_REG_DATA);       /* read the key and put the value into scancode */
            break;
        }
    } while(1);
    if (scancode == 0x2A || scancode == 0x36) shiftFlag = 1; /* check whether the shift key is pressed */
    if (scancode == 0xAA || scancode == 0xB6) shiftFlag = 0; /* check whether the shift key is released */
    if (scancode == 0x1D) ctrlFlag = 1;
    if (scancode == 0x9D) ctrlFlag = 0;
    if (scancode == 0x38) altFlag = 1;
    if (scancode == 0xB8) altFlag = 0;
    if (scancode == 0x3A) {
        if (capsFlag == 0) capsFlag = 1;
        else capsFlag = 0;
    }
    if (scancode == 0x26 && ctrlFlag == 1) {
        setCursor(0, 0);
        clear();
        ctrlFlag = 0;
        return;
    }
    if (scancode == 0x0E) {
        if (buffIdx != 0) {
            buffIdx --;
            keyBuffer[buffIdx] = '\0';
        }
    }
    /* if a key is pressed, decode it into the char that should be print on the screen */
    if (scancode > 0x00 && scancode < 0x81) pressedKey = KB_decode(scancode);
    /* if the key pressed value is known, print it */
    if (pressedKey != 0) {
        printf("%c", pressedKey);
        if (buffIdx < BUFF_SIZE) {
            keyBuffer[buffIdx] = pressedKey;
            buffIdx ++;
        }
        else enterFlag = 1;
    }
    if (pressedKey == 0) spKey(scancode);
    moveCursor();
}

/*
 * KB_decode
 *   DESCRIPTION: decode the signal received by data and return
 *   INPUTS: scancode -- the signal received by data port of keyboard
 *   OUTPUTS: none
 *   RETURN VALUE: the char that should be displayed on the screen
 *   SIDE EFFECTS: none
 */

unsigned char KB_decode(unsigned char scancode) {
    switch(scancode) {
        case 0x1C: {
            enterFlag = 1;
            return '\n';
        }
        case 0x39: return ' ';
    }
    if (shiftFlag == 0) {       /*check whether the shift key is pressed*/
        switch(scancode) {          /*decode the scancode*/
            case 0x02: return '1';
            case 0x03: return '2';
            case 0x04: return '3';
            case 0x05: return '4';
            case 0x06: return '5';
            case 0x07: return '6';
            case 0x08: return '7';
            case 0x09: return '8';
            case 0x0A: return '9';
            case 0x0B: return '0';
            case 0x0C: return '-';
            case 0x0D: return '=';
            case 0x1A: return '[';
            case 0x1B: return ']';
            case 0x27: return ';';
            case 0x28: return '\'';
            case 0x29: return '`';
            case 0x2B: return '\\';
            case 0x33: return ',';
            case 0x34: return '.';
            case 0x35: return '/';
        }
    }
    else {
        switch(scancode) {          /*decode the scancode*/
            case 0x02: return '!';
            case 0x03: return '@';
            case 0x04: return '#';
            case 0x05: return '$';
            case 0x06: return '%';
            case 0x07: return '^';
            case 0x08: return '&';
            case 0x09: return '*';
            case 0x0A: return '(';
            case 0x0B: return ')';
            case 0x0C: return '_';
            case 0x0D: return '+';
            case 0x1A: return '{';
            case 0x1B: return '}';
            case 0x27: return ':';
            case 0x28: return '"';
            case 0x29: return '~';
            case 0x2B: return '|';
            case 0x33: return '<';
            case 0x34: return '>';
            case 0x35: return '?';
        }
    }
    if (shiftFlag ^ capsFlag) {
        switch(scancode) {
            case 0x10: return 'Q';
            case 0x11: return 'W';
            case 0x12: return 'E';
            case 0x13: return 'R';
            case 0x14: return 'T';
            case 0x15: return 'Y';
            case 0x16: return 'U';
            case 0x17: return 'I';
            case 0x18: return 'O';
            case 0x19: return 'P';
            case 0x1E: return 'A';
            case 0x1F: return 'S';
            case 0x20: return 'D';
            case 0x21: return 'F';
            case 0x22: return 'G';
            case 0x23: return 'H';
            case 0x24: return 'J';
            case 0x25: return 'K';
            case 0x26: return 'L';
            case 0x2C: return 'Z';
            case 0x2D: return 'X';
            case 0x2E: return 'C';
            case 0x2F: return 'V';
            case 0x30: return 'B';
            case 0x31: return 'N';
            case 0x32: return 'M';
        }
    }
    else {
        switch(scancode) {
            case 0x10: return 'q';
            case 0x11: return 'w';
            case 0x12: return 'e';
            case 0x13: return 'r';
            case 0x14: return 't';
            case 0x15: return 'y';
            case 0x16: return 'u';
            case 0x17: return 'i';
            case 0x18: return 'o';
            case 0x19: return 'p';
            case 0x1E: return 'a';
            case 0x1F: return 's';
            case 0x20: return 'd';
            case 0x21: return 'f';
            case 0x22: return 'g';
            case 0x23: return 'h';
            case 0x24: return 'j';
            case 0x25: return 'k';
            case 0x26: return 'l';
            case 0x2C: return 'z';
            case 0x2D: return 'x';
            case 0x2E: return 'c';
            case 0x2F: return 'v';
            case 0x30: return 'b';
            case 0x31: return 'n';
            case 0x32: return 'm';
        }
    }
    return 0;                 /*if the pressed key is unkown, print ?*/
}

/*
 * init_keyboard
 *   DESCRIPTION: this function will be called in the kernel.c initializing
                  the keyboard and enable keyboard IRQ
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: initialize the keyboard and enable keyboard IRQ
 */
void init_keyboard() {
    shiftFlag = 0;              /*reset the shift flag*/
    enable_irq(KEYBOARD_IRQ);   /*enable keyboard IRQ*/
}

/*
 * rtc_interrupt
 *   DESCRIPTION: this function will be called by the rtc_wrapper execute
                  the interrupt of rtc
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS:  execute the interrupt of rtc
 */
void rtc_interrupt() {
    cli();                      /*clean the interrupt flag*/
    send_eoi(RTC_IRQ);          /*send the end of interrupt signal to PIC*/
    sti();                      /*restore the interrupt flag*/
    outb(SR_C, RTC_REG_NUM);    /*select register C*/
    inb(RTC_REG_DATA);          /*throw away contents*/
#if (RTC_TEST == 1)
    test_interrupts();
#endif
    rtcFlag = 0;
}

/*
 * init_rtc
 *   DESCRIPTION: this function will be called in the kernel.c, initializing
                  the rtc and enable rtc IRQ
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: initialize the rtc and enable rtc IRQ
 */

void init_rtc() {
    char prev;
    enable_irq(RTC_IRQ);        /*enable rtc IRQ*/
    outb(SR_B, RTC_REG_NUM);        /*select register B, and disable NMI*/
    prev = inb(RTC_REG_DATA);       /*read the current value of register B*/
    outb(SR_B, RTC_REG_NUM);        /*set the index again*/
    outb(prev | PERIOD, RTC_REG_DATA);      /*write the previous value ORed with 0x40. This turns on bit 6 of register B*/
    outb(SR_C, RTC_REG_NUM);        /*select register C*/
    inb(RTC_REG_DATA);      /*throw away contents*/
    set_rate(RATE - 1);     /*set the rate*/
}

/*
 * set_rate
 *   DESCRIPTION: change the rate of rtc freqency
 *   INPUTS: rate -- the new rate of rtc
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: change the rate of rtc freqency
 */

void set_rate(unsigned rate) {
  char prev;
  rate &= MAX_RATE;         /*rate must be above 2 and not over 15*/
  outb(SR_A, RTC_REG_NUM);      /*set index to register A, disable NMI*/
  prev = inb(RTC_REG_DATA);         /*get initial value of register A*/
  outb(SR_A, RTC_REG_NUM);      /*reset index to A*/
  outb((prev & 0xF0) | rate, RTC_REG_DATA);     /*write only our rate to A. Note, rate is the bottom 4 bits*/
}

unsigned char *getBuffer() {return keyBuffer;}

uint8_t getEnter() {return enterFlag;}

void resetEnter() {enterFlag = 0;}

void resetBuffer() {
    int i;
    for (i = 0; i < BUFF_SIZE; i ++) keyBuffer[i] = '\0';
}
