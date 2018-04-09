#include "system_call.h"
#include "file_system.h"
#include "rtc.h"
#include "terminal.h"
#include "paging.h"
#include "lib.h"
#include "loader.h"
#include "pcb.h"
#include "x86_desc.h"

#include "tests.h"

// these following variables act as function jumptables for different files; static is thus safe
static fileOperationTable_t inTable;    // 'stdin' jumptable
static fileOperationTable_t outTable;   // 'stdout' jumptable
static fileOperationTable_t rtcTable;   // 'rtc' jumptable
static fileOperationTable_t dirTable;   // directory jumptable
static fileOperationTable_t regTable;   // regular files jumptable
#define FD_PARAM_LOW   2 // user has no priviledge to close fd 0, 1 or non-exist file
#define FD_PARAM_UPPER  7 // any fd is greater then 7 is invalid

/*
 * open
 *   DESCRIPTION: this function will be called by system call wrapper,
                  open the file of the filename argument
 *   INPUTS: filename -- the filename of the file that needed to open
 *   OUTPUTS: none
 *   RETURN VALUE: fd -- the index of file array
 *   SIDE EFFECTS: change the file descriptor due to the index
 */

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
        if((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[i].flags == STATUS_CLOSED) {
            fullFlag = 0;
            switch (dentry.filetype) {      //get the file type and set the operation table to the correct one
                case 0:
                    fd = (*(rtcTable.oFunc))(filename);
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

/*
 * close
 *   DESCRIPTION: this function will be called by system call wrapper,
                  close the file of the file array index
 *   INPUTS: fd -- the file array index of the file that needed to close
 *   OUTPUTS: 0 --sucess
              -1 -- fail
 *   RETURN VALUE: none
 *   SIDE EFFECTS: close the file of the file array index
 */

int32_t close(int32_t fd) {
    if(fd < FD_PARAM_LOW || fd > FD_PARAM_UPPER){       //check whether fd is valid
        printf("Process close has bad parameter.\n");
        return -1;
    }
    if ((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].flags == STATUS_CLOSED) {        //check whether the file is already closed
        printf("Process tries to close a already closed file.\n");
        return -1;
    }
    if ((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table == NULL){          //check whether the process has the right to close the file
        printf("Process tries to close priviledged file.\n");
        return -1;
    }
    if(-1 == (*((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table->cFunc))(fd)){     //close function fails due to an unkown bug
        printf("UNKOWN BUG IN CLOSE.\n");
        return -1;
    }
    (ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table = NULL;            //reset the file descriptor in the file array
    (ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].inode = 0xFFFF;
    (ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].filePos = 0;
    (ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].flags = STATUS_CLOSED;
    return 0;
}

/*
 * read
 *   DESCRIPTION: this function will be called by system call wrapper,
                  execute the correct read function decided by filetype
 *   INPUTS: fd -- the file array index of the file that needed to read
 *   OUTPUTS: 0 --sucess
              -1 -- fail
 *   RETURN VALUE: none
 *   SIDE EFFECTS: execute the correct read function decided by filetype
 */

int32_t read(int32_t fd, void *buf, int32_t nbytes) {
    if(ece391_process_manager.curr_pid == -1){          //check whether this is process running
        printf("No process is running.\n");
        *((int8_t*) buf) = '\0';
        return -1;
    }
    if ((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].flags == STATUS_CLOSED) {            //check whether the file to read is open
        printf("Process tries to read a already closed file. %d\n", fd);
        *((int8_t*) buf) = '\0';
        return -1;
    }
    if (fd < 0 || fd > FD_PARAM_UPPER) {             //check whether the fd is valid
        printf("Process read has bad parameter.\n");
        *((int8_t*) buf) = '\0';
        return -1;
    }
    if((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table->rFunc == NULL){            //check whether the process has the right to read the file
        printf("File has no priviledge to read.\n");
        *((int8_t*) buf) = '\0';
        return -1;
    }
    {
        return (*((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table->rFunc))(fd, buf, nbytes);           //execute the right read function
    }
}

/*
 * write
 *   DESCRIPTION: this function will be called by system call wrapper,
                  execute the correct write function decided by filetype
 *   INPUTS: fd -- the file array index of the file that needed to write
 *   OUTPUTS: 0 --sucess
              -1 -- fail
 *   RETURN VALUE: none
 *   SIDE EFFECTS: execute the correct write function decided by filetype
 */

int32_t write(int32_t fd, const void *buf, int32_t nbytes) {
    if(ece391_process_manager.curr_pid == -1){          //check whether this is process running
        printf("No process is running.\n");
        return -1;
    }
    if ((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].flags == STATUS_CLOSED) {        //check whether the file to write is open
        printf("Process tries to write to a already closed file.\n");
        return -1;
    }
    if (fd < 0 || fd > FD_PARAM_UPPER) {        //check whether the fd is valid
        printf("Process write has bad parameter. %d\n", fd);
        return -1;
    }
    if((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table->wFunc == NULL){            //check whether the process has the right to write the file
        printf("File has no priviledge to write.\n");
        return -1;
    }
    {
        return (*((ece391_process_manager.process_position[ece391_process_manager.curr_pid-1])->file_array.files[fd].table->wFunc))(fd, buf, nbytes);       //execute the right write function
    }
}

static int32_t halt_ret;

/*
 * halt
 *   DESCRIPTION: this function will be called by system call wrapper,
                  halt the current process
 *   INPUTS: status -- the status
 *   OUTPUTS: none
 *   RETURN VALUE: this function should never be returned
 *   SIDE EFFECTS: halt the current process
 */

int32_t halt (uint8_t status) {
    uint32_t f = ((ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1])->exc_flag);
    if (ece391_process_manager.curr_pid == -1)
        return -1;
    /*need special treatment for the first shell process*/
    if ((ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1])->parent_pid != -1) {
        tss.esp0 = (ece391_process_manager.process_position[(ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1])->parent_pid - 1])->esp;
    }
    else {
        /*need special treatment*/
        tss.esp0+=4;
    }
    /*no longer stores parent_pid*/
    //tss.esp0 = ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->parent_esp;
    user_page_unmapping(ece391_process_manager.curr_pid);
    halt_ret = ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->halt_ebp;
    pop_process();
    /*if still have parent user process*/
    if (ece391_process_manager.curr_pid > 0) {
        user_page_mapping(ece391_process_manager.curr_pid);
    }

    if ( f == HALT_NORM ){
        asm volatile("movzbl %%bl,%%ebx\n\t" : :);
        asm volatile("jmp EXE_RETURN" : :);
    }
    else {
        asm volatile("movl $256,%%ebx\n\t" : :);
        asm volatile("jmp EXE_RETURN" : :);
    }
    printf("error\n");
    return 0;       //prevent warning
}

/*
 * execute
 *   DESCRIPTION: this function will be called by system call wrapper,
                  execute next process
 *   INPUTS: command -- the next process to be executed
 *   OUTPUTS: 0 - normally
              1 - 255 --error
              256 -- exception
 *   RETURN VALUE: none
 *   SIDE EFFECTS: execute the next process
 */

int32_t execute (const uint8_t* command) {
    uint8_t filename[128];
    uint32_t idx = 0;
    int16_t cur_ds;
    int32_t temp;
    int32_t temp_esp;
    /*check for bad input command*/
    if (command == NULL) {
      printf("ERROR: command does not exist.\n");
      return -1;
    }

    if (ece391_process_manager.curr_pid == MAX_PROCESS_NUM) {
        ERROR_MSG;
        printf("Maximum process number reached. Max %d.\n", ece391_process_manager.curr_pid);
        return 1; // Program terminated abnormally
    }

    //int8_t par_pid;

    while (command[idx] != ' ' && command[idx] != '\0')
        idx++;
    memcpy(filename, command, idx);
    filename[idx] = '\0';
    /*checks whether the file is executable*/
    if (check_executable_validity(filename) == -1) {
      printf("ERROR: the file specified is inexecutable.\n");
      return -1;
    }


    /*stores current process ebp, esp into the process manager*/
    /*not right, need to delete*/
    //asm volatile("movl %%ebp,%0\n\t" :"=r" (ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->ebp));
    //asm volatile("movl %%esp,%0\n\t" :"=r" (ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->esp));
    //par_pid = ece391_process_manager.curr_pid;
    /*initialize a pcb for the current process, get the process number*/
    /*current esp, */

    int8_t pid = init_pcb(&ece391_process_manager);
    if (pid < 1) {
      printf("ERROR: unable to create a new pcb.\n");
      return -1;
    }

    //ece391_process_manager.process_position[(ece391_process_manager.pid)-1]->parent_pid = par_pid;
    /*now cur_pid is the new pid*/
    push_process(pid);

    /*initialized user level paging*/
    user_page_mapping(pid);

    /*copy the user image to the user level page*/
    uint32_t* execute_start = load_user_image(filename);

    #if (EXCEPTION_TEST == 1)
    paging_test();
    #endif

    // esp
    asm volatile("movl %%esp,%0\n\t" :"=r" (temp_esp));
    asm volatile("movl %%ebp,%0\n\t" :"=r" (ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->halt_ebp));

    /*code for context switch*/
    if((ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1])->parent_pid == -1){ // if it is the first process, maintain the current esp
      (ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->esp )= temp_esp-4;
    }
    tss.esp0 = ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->esp;
    tss.ss0 = KERNEL_DS;

    /*code for setting up stack for iret*/
    //use the exexute_start to setup eip
    //asm volatile("movl %%ebp,%0\n\t" :"=r" (ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->parent_ebp));
    //asm volatile("movl %%esp,%0\n\t" :"=r" (ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->parent_esp));
    //ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->parent_pid = par_pid;
    asm volatile("movw %%ds,%0\n\t" :"=r" (cur_ds));
    asm volatile("movw %0,%%ax\n\t": :"g" (USER_DS));
    asm volatile("movw %%ax,%%ds\n\t": :);
    /* ss */
    asm volatile("pushl %0\n\t" : :"g" (USER_DS));
    /* esp */
    asm volatile("pushl %0\n\t" : :"g" (LOAD_PAGE_END_ADDR-1));
    /* eflags */
    asm volatile("pushfl\n\t" : :);
    /* cs */
    asm volatile("pushl %0\n\t" : :"g" (USER_CS));
    /* eip*/
    asm volatile("pushl %0\n\t" : :"g" (execute_start));
    asm volatile("iret" : :);
    asm volatile("EXE_RETURN:");

    asm volatile("movl %0, %%ebp\n\t": :"g" (halt_ret));
    // asm volatile("movl %%ebx, %%eax\n\t" : :);
    asm volatile("movl %%ebx,%0\n\t" :"=r" (temp));
    //asm volatile("ret \n\t" : :);
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

/*
 * init_fileArray
 *   DESCRIPTION: this function initialize the file array
 *   INPUTS: new_file_array -- the file array that need to be initialized
 *   OUTPUTS:none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: initialize the file array
 */

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

/*
 * init_file_operation_jumptables
 *   DESCRIPTION: this function initialize the file operation jumptable
                  for terminal, rtc and file
 *   INPUTS: none
 *   OUTPUTS:none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: initialize the file operation jumptable
 */

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
