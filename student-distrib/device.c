#include "device.h"
#include "i8259.h"

static uint8_t shiftFlag;

void keyboard_interupt() {
    char scancode;
    char pressedKey;
    while (!(inb(0x64) & 1));
    scancode = inb(0x60);
    pressedKey = KB_decode(scancode);
    printf("%c", pressedKey);
}

char KB_decode(char scancode) {
    if (shiftFlag == 0) {
        switch(scancode) {
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
            case 0x0E: return ' ';
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
            case 0x1A: return '[';
            case 0x1B: return ']';
            case 0x1C: return '\n';
            case 0x1E: return 'a';
            case 0x1F: return 's';
            case 0x20: return 'd';
            case 0x21: return 'f';
            case 0x22: return 'g';
            case 0x23: return 'h';
            case 0x24: return 'j';
            case 0x25: return 'k';
            case 0x26: return 'l';
            case 0x27: return ';';
            case 0x28: return '\'';
            case 0x29: return '`';

        }
    }
    return ' ';

}

void init_keyboard() {
    shiftFlag = 0;
    enable_irq(KEYBOARD_IRQ);
}
