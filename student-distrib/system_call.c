#include "system_call.h"
#include "file_system.h"
#include "rtc.h"
#include "terminal.h"
#include "paging.h"
#include "lib.h"
#include "loader.h"
#include "pcb.h"
#include "x86_desc.h"

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
    printf("\nCP_0\n");
    // check if file status array has spare spot for new file to open
    fullFlag = 1;
    for (i = FD_PARAM_LOW; i <= FD_PARAM_UPPER; i ++) {
        // NOTE: (pid-1) is the index of this array, and this is a !pointer! array
        if((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[i].flags == STATUS_CLOSED) {
            fullFlag = 0;
            switch (dentry.filetype) {
                case 0:
                    printf("\nCP_1%#x\n", (rtcTable.oFunc));
                    fd = (*(rtcTable.oFunc))(filename);
                    printf("\nCP_2%d\n",fd);
                    (ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table = &rtcTable;
                    break;
                case 1:
                    fd = (*(dirTable.oFunc))(filename);
                    (ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table = &dirTable;
                    break;
                case 2:
                    fd = (*(regTable.oFunc))(filename);
                    (ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table = &regTable;
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
    if(fd < FD_PARAM_LOW || fd > FD_PARAM_UPPER){
        printf("Process close has bad parameter.\n");
        return -1;
    }
    if ((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].flags == STATUS_CLOSED) {
        printf("Process tries to close a already closed file.\n");
        return -1;
    }
    if ((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table == NULL){
        printf("Process tries to close priviledged file.\n");
        return -1;
    }
    if(-1 == (*((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table->cFunc))(fd)){
        printf("UNKOWN BUG IN CLOSE.\n");
        return -1;
    }
    (ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table = NULL;
    (ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].inode = 0xFFFF;
    (ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].filePos = 0;
    (ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].flags = STATUS_CLOSED;
    return 0;
}

int32_t read(int32_t fd, void *buf, int32_t nbytes) {
    if(ece391_process_manager.curr_pid == -1){
        printf("No process is running.\n");
        *((int8_t*) buf) = '\0';
        return -1;
    }
    if ((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].flags == STATUS_CLOSED) {
        printf("Process tries to read a already closed file.\n");
        *((int8_t*) buf) = '\0';
        return -1;
    }
    if (fd < 0 || fd > FD_PARAM_UPPER) {
        printf("Process read has bad parameter.\n");
        *((int8_t*) buf) = '\0';
        return -1;
    }
    if((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table->rFunc == NULL){
        printf("File has no priviledge to read.\n");
        *((int8_t*) buf) = '\0';
        return -1;
    }
    {
        return (*((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table->rFunc))(fd, buf, nbytes);
    }
}

int32_t write(int32_t fd, const void *buf, int32_t nbytes) {
    if(ece391_process_manager.curr_pid == -1){
        printf("No process is running.\n");
        return -1;
    }
    if ((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].flags == STATUS_CLOSED) {
        printf("Process tries to write to a already closed file.\n");
        return -1;
    }
    if (fd < 0 || fd > FD_PARAM_UPPER) {
        printf("Process read has bad parameter.\n");
        return -1;
    }
    if((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table->wFunc == NULL){
        printf("File has no priviledge to write.\n");
        return -1;
    }
    {
        return (*((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table->wFunc))(fd, buf, nbytes);
    }
}

int32_t halt (uint8_t status) {
    tss.esp0 = ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->parent_esp;
    user_page_unmapping(ece391_process_manager.curr_pid);
    pop_process();
    asm volatile("movzbl %%bl,%%ebx\n\t" : :);
    asm volatile("jmp EXE_RETURN" : :);
    printf("error\n");
    return 0;       //prevent warning
}
int32_t execute (const uint8_t* command) {
    /*check for bad input command*/
    if (command == NULL) {
      printf("ERROR: command does not exist.\n");
      return -1;
    }

    uint8_t *filename;
    uint32_t idx = 0;
    int16_t cur_ds;
    int32_t temp;
    int8_t par_pid;

    while (command[idx] != ' ' && command[idx] != '\0')
        idx++;
    memcpy(filename, command, idx);

    /*checks whether the file is executable*/
    if (check_executable_validity(filename) == -1) {
      printf("ERROR: the file specified is inexecutable.\n");
      return -1;
    }


    /*stores current process ebp, esp into the process manager*/
    asm volatile("movl %%ebp,%0\n\t" :"=r" (ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->ebp));
    asm volatile("movl %%esp,%0\n\t" :"=r" (ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->esp));
    par_pid = ece391_process_manager.curr_pid;
    /*initialize a pcb for the current process, get the process number*/
    int8_t pid = init_pcb(&ece391_process_manager);
    if (pid < 1) {
      printf("ERROR: unable to create a new pcb.\n");
      return -1;
    }

    push_process(pid);

    /*initialized user level paging*/
    user_page_mapping(pid);

    /*copy the user image to the user level page*/
    uint32_t* execute_start = load_user_image(filename);

    /*code for context switch*/
    tss.esp0 = ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->esp;
    tss.ss0 = KERNEL_DS;

    /*code for setting up stack for iret*/
    //use the exexute_start to setup eip
    asm volatile("movl %%ebp,%0\n\t" :"=r" (ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->parent_ebp));
    asm volatile("movl %%esp,%0\n\t" :"=r" (ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->parent_esp));
    ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->parent_pid = par_pid;
    asm volatile("movw %%ds,%0\n\t" :"=r" (cur_ds));
    asm volatile("movw %0,%%ax\n\t": :"g" (USER_DS));
    asm volatile("movw %%ax,%%ds\n\t": :);
    asm volatile("pushw %0\n\t" : :"g" (USER_DS));
    asm volatile("pushl %0\n\t" : :"g" (ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->esp));
    asm volatile("pushfl\n\t" : :);
    asm volatile("movl %%eax,%0\n\t" :"=r" (temp));
    asm volatile("popl %%eax\n\t" : :);
    asm volatile("orl $0x200, %%eax\n\t" : :);
    asm volatile("pushl %%eax\n\t" : :);
    asm volatile("movl %0,%%eax": :"r" (temp));
    asm volatile("pushw %0\n\t" : :"g" (USER_CS));
    asm volatile("pushl %0\n\t" : :"g" (execute_start));
    asm volatile("iret" : :);
    asm volatile("EXE_RETURN:");
    asm volatile("movl %%ebx,%0\n\t" :"=r" (temp));
    return temp;
}

// the following funcions are not implemented
int32_t getargs (uint8_t* buf, int32_t nbytes) {return 0;}
int32_t vidmap (uint8_t** screen_start) {return 0;}
int32_t set_handler (int32_t signum, void* handler_address) {return 0;}
int32_t sigreturn (void) {return 0;}

// this funcion initilize the file array, automatically open the
// terminal open/close
// this function is called by PCB
void init_fileArray(fileArray_t* new_file_array){
    int32_t ii; // for traverse the file array
    // open stdin automatically
    new_file_array->files[0].table = &inTable;
    new_file_array->files[0].inode = 0xFFFF;  // no such file exist in
    new_file_array->files[0].filePos = 0;
    new_file_array->files[0].flags = STATUS_OPENED;
    // open stdout automatically
    new_file_array->files[1].table = &outTable;
    new_file_array->files[1].inode = 0xFFFF;  // no such file exist in
    new_file_array->files[1].filePos = 0;
    new_file_array->files[1].flags = STATUS_OPENED;
    // open terminal
    terminal_open((uint8_t *)"stdIO");
    // traverse array to set rest files' status
    for (ii = 2; ii < FA_SIZE; ii++)
        new_file_array->files[ii].flags = STATUS_CLOSED;
    // make sure that the all the drivers are interacting with the correct file array

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
