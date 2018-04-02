#include "system_call.h"
#include "file_system.h"
#include "rtc.h"
#include "terminal.h"
#include "paging.h"
#include "lib.h"

// these following variables act as function jumptables for different files; static is thus safe
static fileOperationTable_t inTable;    // 'stdin' jumptable
static fileOperationTable_t outTable;   // 'stdout' jumptable
static fileOperationTable_t rtcTable;   // 'rtc' jumptable
static fileOperationTable_t dirTable;   // directory jumptable
static fileOperationTable_t regTable;   // regular files jumptable

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

int32_t halt (uint8_t status);
int32_t execute (const uint8 t* command);
// the following funcions are not implemented
int32_t getargs (uint8_t* buf, int32_t nbytes);
int32_t vidmap (uint8_t** screen_start);
int32_t set_handler (int32_t signum, void* handler_address);
int32_t sigreturn (void);

// this funcion initilize the file array, automatically open the
// terminal open/close
// this function is called by PCB
void init_fileArray(fileArray_t* new_file_array){
    int32_t ii; // for traverse the file array
    // open stdin automatically
    new_file_array->files[0].table = &inTable;
    new_file_array->files[0].inode = NULL;  // no such file exist in
    new_file_array->files[0].filePos = 0;
    new_file_array->files[0].flag = STATUS_OPENED;
    // open stdout automatically
    new_file_array->files[1].table = &outTable;
    new_file_array->files[1].inode = NULL;  // no such file exist in
    new_file_array->files[1].filePos = 0;
    new_file_array->files[1].flag = FILE_EXIST;
    // traverse array to set rest files' status
    for (ii = 2; ii < FA_SIZE; ii++)
        new_file_array->files[ii].flag = STATUS_CLOSED;
    // make sure that the all the drivers are interacting with the correct file array
    fileStatusArray = new_file_array;

}
// this function should be called once,
// NOTE can is called in function : void init_process_manager(process_manager_t* processManager)
void init_file_operation_jumptables(void){
    // init the local jumptables
    // 'stdin' jumptable
    inTable.oFunc = &terminal_open;
    inTable.cFunc = &terminal_close;
    inTable.rFunc = &terminal_read;
    inTable.wFunc = NULL;
    // 'stdout' jumptable
    outTable.oFunc = &terminal_open;
    outTable.cFunc = &terminal_close;
    outTable.rFunc = NULL;
    outTable.wFunc = &terminal_write;
    // 'rtc' jumptable
    rtcTable.oFunc = &rtc_open;
    rtcTable.cFunc = &rtc_close;
    rtcTable.rFunc = &rtc_read;
    rtcTable.wFunc = &rtc_write;
    // directory jumptable
    dirTable.oFunc = &dir_open;
    dirTable.cFunc = &dir_close;
    dirTable.rFunc = &dir_read;
    dirTable.wFunc = &dir_write;
    // regular files jumptable
    regTable.oFunc = &file_open;
    regTable.cFunc = &file_close;
    regTable.rFunc = &file_read;
    regTable.wFunc = &file_write;
}
