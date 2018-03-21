#include "terminal.h"
#include "device.h"
#include "i8259.h"

int32_t terminal_open() {
    return 0;
}

int32_t terminal_close() {
    return 0;
}

int32_t terminal_read(unsigned char *buffer, int32_t bytes) {
    if (buffer == NULL) return -1;
    if (bytes < 0) return -1;
    int i;
    unsigned char *keyBuffer;
    uint32_t buffLen;
    while (1) {
        keyBuffer = getBuffer();
        if (keyBuffer != NULL) {
            while (!getEnter())
            resetEnter();
            cli();
            buffLen = strlen((int8_t *)keyBuffer);
            for (i = 0; i < bytes; i ++) buffer[i] = '\0';
            if (buffLen < bytes) bytes = buffLen;
            memcpy(buffer, keyBuffer, bytes);
            resetBuffer();
            sti();
            return bytes;
        }
    }
    return -1;
}

int32_t terminal_write(unsigned char *buffer, int32_t bytes) {
    if (buffer == NULL) return -1;
    if (bytes < 0) return -1;
    int i;
    uint32_t buffLen = strlen((int8_t *)buffer);
    if (buffLen < bytes) bytes = buffLen;
    for (i = 0; i < bytes; i ++) printf("%c", buffer[i]);
    return bytes;
}
