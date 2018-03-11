#ifndef DEVICE_H
#define DEVICE_H

#include "types.h"
#include "x86_desc.h"
#include "lib.h"


void keyboard_interupt();
char KB_decode(char scancode);
void init_keyboard();
#endif
