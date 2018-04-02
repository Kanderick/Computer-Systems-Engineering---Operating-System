#include "system_call.h"
#include "file_system.h"
#include "rtc.h"
#include "terminal.h"
#include "paging.h"
#include "lib.h"

static fileArray_t array;


int32_t open(const uint8_t *filename) {
    int i;
    dentry_t dentry;
    if (filename == NULL) return -1;
    if (read_dentry_by_name(filename, &dentry) == -1) return -1;
    /* WTF is this */
    if (filename == (uint8_t *)"stdin") return 0;
    if (filename == (uint8_t *)"stdout") return 1;
    array.fullFlag = 1;
    for (i = 2; i < 8; i ++) {
        if(array.files[i].flags == 0) {
            array.fullFlag = 0;
            array.files[i].flags = 1;
            break;
        }
    }
    if (array.fullFlag == 1) return -1;
    switch (dentry.filetype) {
        case 0:
            array.files[i].table = &rtcTable;
            array.files[i].inode = NULL;
            array.files[i].filePos = 0;
            break;
        // case 1:
        //     array.files[i].table = &dirTable;
        //     array.files[i].inode = NULL;
        //     array.files[i].filePos = 0;
        //     break;
        // case 2:
        //     array.inode = (uint32_t *)(ece391FileSystem.ece391_boot_block + BLOCK_SIZE_4KB * (dentry.inode_num + 1));
        //     array.files[i].table = &regTable;
        //     array.files[i].inode = array.inode;
        //     array.files[i].filePos = 0;
        //     break;
    }
    array.files[i].table->fotOpen(filename);
    return i;
}

int32_t close(int32_t fd) {
    if (array.files[fd].flags == 0) return -1;
    if (array.files[fd].table != NULL)
        array.files[fd].table->fotClose(fd);
    array.files[fd].table = NULL;
    array.files[fd].inode = NULL;
    array.files[fd].filePos = 0;
    array.files[fd].flags = 0;
    return 0;
}

int32_t read(int32_t fd, void *buf, int32_t nbytes) {
    if (fd >= 0 && fd < 8) {
        return array.files[fd].table->fotRead(fd, buf, nbytes);
    }
    return -1;
}

int32_t write(int32_t fd, const void *buf, int32_t nbytes) {
    if (fd >= 0 && fd < 8) {
        return array.files[fd].table->fotWrite(fd, buf, nbytes);
    }
    return -1;
}

void init_fot(void) {
    inTable.fotOpen = (oFunc)&terminal_open;
    inTable.fotClose = (cFunc)&terminal_close;
    inTable.fotRead = (rFunc)&terminal_read;
    inTable.fotWrite = NULL;

    outTable.fotOpen = (oFunc)&terminal_open;
    outTable.fotClose = (cFunc)&terminal_close;
    outTable.fotRead = NULL;
    outTable.fotWrite = (wFunc)&terminal_write;

    rtcTable.fotOpen = (oFunc)&rtc_open;
    rtcTable.fotClose = (cFunc)&rtc_close;
    rtcTable.fotRead = (rFunc)&rtc_read;
    rtcTable.fotWrite = (wFunc)&rtc_write;
}
