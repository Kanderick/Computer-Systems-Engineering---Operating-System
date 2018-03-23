#include "terminal.h"
#include "device.h"
#include "i8259.h"

/*
 * terminal_open
 *   DESCRIPTION: terminal open function
 *   INPUTS: filename -- the file name of the terminal
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: open the terminal file
 */
int32_t terminal_open(uint8_t *filename) {
    return 0;
}

/*
 * terminal_close
 *   DESCRIPTION: terminal close function
 *   INPUTS: fd -- file discriptor number for the flie array
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: close the terminal file
 */
int32_t terminal_close(int32_t fd) {
    return 0;
}

/*
 * terminal_read
 *   DESCRIPTION: terminal read function, read the keyboard to
                  the target buffer
 *   INPUTS: fd -- file discriptor number for the flie array
             buffer -- the keyboard buffer
             bytes -- the number of bytes that need to be read in
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: read the keyboard to the target buffer
 */
int32_t terminal_read(int32_t fd, unsigned char *buffer, int32_t bytes) {
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

/*
 * terminal_write
 *   DESCRIPTION: terminal write function, print the char in
                  target buffer onto the screen
 *   INPUTS: fd -- file discriptor number for the flie array
             buffer -- the keyboard buffer
             bytes -- the number of bytes that need to be read in
 *   OUTPUTS: the chars in the target buffer
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the char in target buffer onto the screen
 */
int32_t terminal_write(int32_t fd, unsigned char *buffer, int32_t bytes) {
    if (buffer == NULL) return -1;
    if (bytes < 0) return -1;
    int i;
    uint32_t buffLen = strlen((int8_t *)buffer);
    if (buffLen < bytes) bytes = buffLen;
    for (i = 0; i < bytes; i ++) printf("%c", buffer[i]);
    return bytes;
}
