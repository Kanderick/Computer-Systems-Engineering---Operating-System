#ifndef DEVICE_H
#define DEVICE_H

#include "types.h"
#include "x86_desc.h"
#include "lib.h"


void keyboard_interrupt();
unsigned char KB_decode(unsigned char scancode);
void init_keyboard();
void rtc_interrupt();
void init_rtc();

#endif
