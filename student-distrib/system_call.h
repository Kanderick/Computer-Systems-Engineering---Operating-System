#ifndef SYSTEM_CALL_H
#define SYSTEM_CALL_H

#include "types.h"
#include "file_system.h"
// total number of files a single PCB is holding
#define FA_SIZE                 8
#define STATUS_CLOSED   67      // 'C' indecates file closed
#define STATUS_OPENED   79      // 'O' indicates file opened
// the file operation table should be contained in each file struct in the file array
typedef struct fileOperationTable {
    // definition of file operation table function pointers, currently we only have four
    int32_t (*oFunc)(const uint8_t *filename);
    int32_t (*cFunc)(int32_t fd);
    int32_t (*rFunc)(int32_t fd, unsigned char *buf, int32_t nbytes);
    int32_t (*wFunc)(int32_t fd, const unsigned char *buf, int32_t nbytes);
} fileOperationTable_t;

// the file struct in the file array
typedef struct ece391_file {
    fileOperationTable_t *table;
    inode_t *inode;
    uint32_t filePos;
    uint32_t flag;
} ece391_file_t;

// the file array should be used and initialze whenever a new PCB is init
typedef struct fileArray {
    ece391_file_t files[FA_SIZE];
    // reserved for other variables

} fileArray_t;

// init the file array passed in by pointer
void init_fileArray(fileArray_t* new_file_array);

// this function should be called once,
// NOTE can is called in function : void init_process_manager(process_manager_t* processManager)
void init_file_operation_jumptables(void);

// declare all the function to prevent system_call wrapper from complaining
int32_t halt (uint8_t status);
int32_t execute (const uint8 t* command);
int32_t read (int32_t fd, void* buf, int32_t nbytes);
int32_t write (int32_t fd, const void* buf, int32_t nbytes);
int32_t open (const uint8_t* filename);
int32_t close (int32_t fd);

// the following funcions are not implemented
int32_t getargs (uint8_t* buf, int32_t nbytes);
int32_t vidmap (uint8_t** screen_start);
int32_t set_handler (int32_t signum, void* handler_address);1
int32_t sigreturn (void);

#endif
