#ifndef DEVICE_H
#define DEVICE_H

#include "types.h"
#include "x86_desc.h"
#include "lib.h"


void keyboard_interrupt(void);
unsigned char KB_decode(unsigned char scancode);
void init_keyboard(void);
void rtc_interrupt(void);
void init_rtc(void);
void set_rate(unsigned rate);

#endif
