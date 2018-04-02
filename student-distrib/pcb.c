#include "pcb.h"

// this is the ece391 process/task manager, all the process info can be found in this data structure
process_manager_t ece391_process_manager;

/* need header */
void init_process_manager(process_manager_t* processManager){
    uint32_t ii; // for traverse array in processManager
    // check if the passed-in pointer is valid
    if (processManager == NULL)
        return;
    // no process is running when the processManager is initialized
    processManager->curr_pid = -1;
    // initialze the process_status in the processManager
    for (ii = 0; ii < MAX_PROCESS_NUM; ii++)
        processManager->process_status[ii] = PROCESS_NOT_EXIST;
    // NOTE just for convinience, call init_file_operation_jumptables here
    init_file_operation_jumptables();
}

// NOTE: this function should only be called when a current process wants to have a child process
int8_t init_pcb(process_manager_t* processManager){
    uint32_t ii; // for traverse array in processManager
    int8_t ret_pid = -1;    // init pid to be invalid
    // check if the passed-in pointer is valid
    if (processManager == NULL)
        return ret_pid;
    for (ii = 0; ii < MAX_PROCESS_NUM; ii++){
        // if there is a spare process position
        if(processManager->process_status[ii] == PROCESS_NOT_EXIST){
            // set returned pid
            ret_pid = ii+1;
            // update the process position: PCB starts at 8MB – 8KB * pid
            processManager->process_position[ii] = (pcb_t*) (PCB_BASE_ADDR - ret_pid*PCB_SEG_LENGTH);
            // init the file_array
            init_fileArray(&(processManager->process_position[ii]->file_array));
            // update the kernel stack for this process
            processManager->process_position[ii]->ebp = (pcb_t*) (PCB_BASE_ADDR - ii*PCB_SEG_LENGTH);
            processManager->process_position[ii]->esp = (pcb_t*) (PCB_BASE_ADDR - ii*PCB_SEG_LENGTH);
            // load parent pid
            processManager->process_position[ii]->parent_pid = processManager->curr_pid;
            // check if to
            if (processManager->curr_pid == -1){
                /* reserved for load root PCB's parent info, esp or ebp, ... */
                break;
            }

            /* reserved for load PCB's parent info, esp or ebp, ... */

        // if there is a spare process position then no need to find next
        break;
        }
    }
    return ret_pid;
}
