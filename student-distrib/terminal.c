#include "terminal.h"
#include "device.h"
#include "i8259.h"

static volatile uint8_t terminalFlag = 0;

/*
 * terminal_open
 *   DESCRIPTION: terminal open function
 *   INPUTS: filename -- the file name of the terminal
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: open the terminal file
 */
int32_t terminal_open(const uint8_t *filename) {
    terminalFlag = 1;
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
    if (terminalFlag == 0) {        /*check whether the terminal is closed*/
        printf("Terminal has already closed.\n");
        return -1;
    }
    terminalFlag = 0;
    return 0;
}

/*
 * terminal_read
 *   DESCRIPTION: terminal read function, read the keyboard to
                  the target buffer
 *   INPUTS: fd -- file discriptor number for the flie array
             buf -- the keyboard buffer
             nbytes -- the number of bytes that need to be read in
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: read the keyboard to the target buffer
 */
int32_t terminal_read(int32_t fd, unsigned char *buf, int32_t nbytes) {
    if (terminalFlag == 0) {             /*check whether the terminal is open*/
        printf("Terminal is not yet opened.\n");
        return -1;
    }
    if (buf == NULL) return -1;         /*check whether the buffer is valid*/
    if (nbytes < 0) return -1;          /*check whether nbytes is valid*/
    int i;
    unsigned char *keyBuffer;
    uint32_t buffLen;
    while (1) {
        keyBuffer = getBuffer();        /*get the key buffer*/
        if (keyBuffer != NULL) {
            while (!getEnter()) {}      /*wait for enter*/
            resetEnter();               /*reset the enter flag*/
            cli();
            buffLen = strlen((int8_t *)keyBuffer);              /*get the length of the string*/
            for (i = 0; i < nbytes; i ++) buf[i] = '\0';        /*initialze the target buffer*/
            if (buffLen < nbytes) nbytes = buffLen;             /*check the length of the string that should be copied*/
            memcpy(buf, keyBuffer, nbytes);                     /*memory copy of the buffer*/
            resetBuffer();                                      /*reset the key buffer*/
            sti();
            return nbytes;                                      /*return the number of bytes that copied*/
        }
    }
    return -1;
}

/*
 * terminal_write
 *   DESCRIPTION: terminal write function, print the char in
                  target buffer onto the screen
 *   INPUTS: fd -- file discriptor number for the flie array
             buf -- the keyboard buffer
             nbytes -- the number of bytes that need to be read in
 *   OUTPUTS: the chars in the target buffer
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the char in target buffer onto the screen
 */
int32_t terminal_write(int32_t fd, const unsigned char *buf, int32_t nbytes) {
    if (terminalFlag == 0) {             /*check whether the terminal is open*/
        printf("Terminal is not yet opened.\n");
        return -1;
    }
    if (buf == NULL) return -1;         /*check whether the buffer is valid*/
    if (nbytes < 0) return -1;          /*check whether nbytes is valid*/
    int i;
    uint32_t buffLen = nbytes;               /*get the length of the string*/
    if (buffLen < nbytes) nbytes = buffLen;                 /*check the length of the string that should be copied*/
    for (i = 0; i < nbytes; i ++) printf("%c", buf[i]);     /*print the string in the buffer onto screen*/
    return nbytes;                                          /*return the number of bytes printed*/
}
