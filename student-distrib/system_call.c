#include "system_call.h"
#include "file_system.h"
#include "paging.h"
#include "lib.h"

static fa array;

int32_t open(const uint8_t *filename) {
    int i;
    dentry_t dentry;
    if (read_dentry_by_name(filename, &dentry) == -1) return -1;
    if ((*filename) == "stdin") return 0;
    if ((*filename) == "stdout") return 1;
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
        case 1:
            array.files[i].table = &dirTable;
            array.files[i].inode = NULL;
            array.files[i].filePos = 0;
            break;
        case 2:
            ece391FileSystem.ece391_inodes = (inode_t *)(ece391FileSystem.ece391_boot_block + BLOCK_SIZE_4KB * (dentry.inode_num + 1));
            array.files[i].table = &regTable;
            array.files[i].inode = (uint32_t *)ece391FileSystem.ece391_inodes;
            array.files[i].filePos = 0;
            break;
    }
    return i;
}

int32_t close(int32_t fd) {
    if (array.files[fd].flags == 0) return -1;
    array.files[fd].table = NULL;
    array.files[fd].inode = NULL;
    array.files[fd].filePos = 0;
    array.files[fd].flags = 0;
    return 0;
}

int32_t read(int32_t fd, void *buf, int32_t nbytes) {
    return 0;
}

int32_t write(int32_t fd, const void *buf, int32_t nbytes) {
    return 0;
}
