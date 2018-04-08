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
#define FD_PARAM_LOW   2 // user has no priviledge to close fd 0, 1 or non-exist file
#define FD_PARAM_UPPER  7 // any fd is greater then 7 is invalid
int32_t open(const uint8_t *filename) {
    int i;
    int fd = -1;
    dentry_t dentry;
    int8_t fullFlag;
    // check if the processManager has a process to run
    if(ece391_process_manager.curr_pid == -1){
        printf("No process is running.\n");
        return -1;
    }
    // check bad param
    if (filename == NULL) {
        printf("Process open file has bad parameter.\n");
        return -1;
    }
    // check if the passed-in file exists in the file system
    if (read_dentry_by_name(filename, &dentry) == -1) {
        printf("Process wants to open invalid file.\n");
        return -1;
    }
    // check if file status array has spare spot for new file to open
    fullFlag = 1;
    for (i = FD_PARAM_LOW; i <= FD_PARAM_UPPER; i ++) {
        // NOTE: (pid-1) is the index of this array, and this is a !pointer! array
        if(ece391_process_manager.process_position[ece391_process_manager.curr_pid-1]->file_array.files[i].flags == STATUS_CLOSED) {
            fullFlag = 0;
            switch (dentry.filetype) {
                case 0:
                    fd = (*(rtcTable->fotOpen))(filename);
                    break;
                case 1:
                    fd = (*(dirTable->fotOpen))(filename);
                    break;
                case 2:
                    fd = (*(regTable->fotOpen))(filename);
                    break;
            }
            break;
        }
    }
    // if no space, then return on error
    if (fullFlag == 1) {
        printf("Process file_array is full.\n");
        return -1;
    }
    return fd;
}

int32_t close(int32_t fd) {
    if(fd < FD_GOOD_PARAM || fd > FD_PARAM_UPPER){
        printf("Process close has bad parameter.\n");
        return -1;
    }
    if (ece391_process_manager.process_position[ece391_process_manager.curr_pid-1]->file_array.files[fd].flags == STATUS_CLOSED) {
        printf("Process tries to close a already closed file.\n");
        return -1;
    }
    if (ece391_process_manager.process_position[ece391_process_manager.curr_pid-1]->file_array.files[fd].table == NULL){
        printf("Process tries to close priviledged file.\n");
        return -1;
    }
    if(-1 == (*(ece391_process_manager.process_position[ece391_process_manager.curr_pid-1]->file_array.files[fd].table->fotClose))(fd)){
        printf("UNKOWN BUG IN CLOSE.\n");
        return -1;
    }
    ece391_process_manager.process_position[ece391_process_manager.curr_pid-1]->file_array.files[fd].table = NULL;
    ece391_process_manager.process_position[ece391_process_manager.curr_pid-1]->file_array.files[fd].inode = NULL;
    ece391_process_manager.process_position[ece391_process_manager.curr_pid-1]->file_array.files[fd].filePos = 0;
    ece391_process_manager.process_position[ece391_process_manager.curr_pid-1]->file_array.files[fd].flags = STATUS_CLOSED;
    return 0;
}

int32_t read(int32_t fd, void *buf, int32_t nbytes) {
    if(ece391_process_manager.curr_pid == -1){
        printf("No process is running.\n");
        return -1;
    }
    if (fd <= 0 || fd > FD_PARAM_UPPER) {
        printf("Process read has bad parameter.\n");
        return -1;
    }
    if(ece391_process_manager.process_position[ece391_process_manager.curr_pid-1]->file_array.files[fd].table->fotRead == NULL){
        printf("File has no priviledge to read.\n");
        return -1;
    }
    {
        return (*(ece391_process_manager.process_position[ece391_process_manager.curr_pid-1]->file_array.files[fd].table->fotRead))(fd, buf, nbytes);
    }
}

int32_t write(int32_t fd, const void *buf, int32_t nbytes) {
    if(ece391_process_manager.curr_pid == -1){
        printf("No process is running.\n");
        return -1;
    }
    if (fd <= 0 || fd > FD_PARAM_UPPER) {
        printf("Process read has bad parameter.\n");
        return -1;
    }
    if(ece391_process_manager.process_position[ece391_process_manager.curr_pid-1]->file_array.files[fd].table->fotWrite == NULL){
        printf("File has no priviledge to write.\n");
        return -1;
    }
    {
        return (*(ece391_process_manager.process_position[ece391_process_manager.curr_pid-1]->file_array.files[fd].table->fotWrite))(fd, buf, nbytes);
    }
}

int32_t halt (uint8_t status);
int32_t execute (const uint8_t* command) {
    if (command == NULL)
        return -1;
    uint8_t *filename;
    uint32_t idx = 0;
    while (command[idx] != ' ' && command[idx] != '/0')
        idx++;
    memcpy(filename, command, idx);
    file_open(filename);

}
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
    new_file_array->files[1].flag = STATUS_OPENED;
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
    inTable.oFunc = NULL;
    inTable.cFunc = NULL;
    inTable.rFunc = &terminal_read;
    inTable.wFunc = NULL;
    // 'stdout' jumptable
    outTable.oFunc = NULL;
    outTable.cFunc = NULL;
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
