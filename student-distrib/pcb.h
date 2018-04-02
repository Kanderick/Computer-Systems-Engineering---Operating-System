#ifndef PCB_H
#define PCB_H

#include "paging.h"     // for paging info to add
#include "system_call.h"    // for init operation jumptables

#define MAX_PROCESS_NUM     2     // for now, set the process number upper limit to 2
// following two are for ece391_process_manager.process_status
#define PROCESS_EXIST       1
#define PROCESS_NOT_EXIST   0
// constant for initialzing pcb
#define PCB_BASE_ADDR       0x800000    // the base address for storing PCB, 8MB
#define PCB_SEG_LENGTH      0x2000      // each segment should be offset from the base, 8KB
// make it packed to spare space for kernel stack
struct process_control_block {
    /* the file array for each process */
    fileArray_t file_array;
    /* parent process statics */
    uint32_t parent_ebp;
    uint32_t parent_esp;
    int8_t parent_pid;     // NOTE: for ece391 PCB, 0..127 pid is enough
    /* current process statics */
    uint32_t ebp;
    uint32_t esp;
    // int8_t pid;          NOTE: no need for store pid again
    /* reserved for paging info */
    uint32_t page_directory_index;
}  __attribute((packed));
typedef struct process_control_block pcb_t;

typedef struct process_manager{
    // NOTE: (pid-1) is the index of this array, and this is a !pointer! array
    // each element points to the position of a PCB
    pcb_t* process_position[MAX_PROCESS_NUM];
    // This array indicates each process presents or not
    uint8_t process_status[MAX_PROCESS_NUM];
    // indicates current process, -1 on no process
    int8_t curr_pid;
}process_manager_t;

// this is the ece391 process/task manager, all the process info can be found in this data structure
extern process_manager_t ece391_process_manager;

// this function initializes the ece391init_process_manager
void init_process_manager(process_manager_t* processManager);

// this function init a new process, return a pid and -1 on failure
int8_t init_pcb(process_manager_t* processManager);

#endif
