#include "multi_terminal.h"
#include "system_call.h"
#include "types.h"
#include "lib.h"
#include "pcb.h"
// current terminal number
uint8_t cur_ter_num;

// ece391 terminal info array
MULTI_TER_INFO_t ece391_multi_ter_info[TER_MAX];

// ece391 terminal status array
uint8_t ece391_multi_ter_status[TER_MAX];

// init multi-terminal
void multi_terminal_init(){
    uint8_t ii, i;
    for (ii = 0; ii < TER_MAX; ii++) {
        ece391_multi_ter_status[ii] = TER_NOT_EXIST;
        ece391_multi_ter_info[ii].ter_screen_x = TER_NOT_EXIST;
        ece391_multi_ter_info[ii].ter_screen_y = TER_NOT_EXIST;
        ece391_multi_ter_info[ii].ter_bufferIdx = TER_NOT_EXIST;
        for (i = 0; i < ter_buffer_len+1; i ++)
            ece391_multi_ter_info[ii].ter_buffer[i] = '\0'; // +1 since we need to detect ENTER after filled
    }
    // open terminal 0
    cur_ter_num = 0;
    ece391_multi_ter_status[cur_ter_num] = TER_EXIST;
    ece391_multi_ter_info[cur_ter_num].Parent_ter = -1; // set Parent_ter to nonsense

}

// switch_terminal function
void switch_terminal(uint32_t next_terminal) {
    int8_t next_ter_number, ii;

    // extract the terminal number to jump to
    if (next_terminal == TO_DESTI) {
        next_ter_number = ece391_multi_ter_info[(uint32_t)cur_ter_num].Dest_ter;
    }
    else if (next_terminal == TO_PARENT) {
        next_ter_number = ece391_multi_ter_info[(uint32_t)cur_ter_num].Parent_ter;
    }
    else {
        return ;
    }
    // handle invalid terminal number
    if(next_terminal<0 || next_terminal>2){
        ERROR_MSG;
        printf("INVALID TERMINAL NUMBER");
        while(1);
    }
    // handle the parent terminal not exist
    if(next_terminal == TO_PARENT && ece391_multi_ter_status[(uint32_t)next_ter_number] == TER_NOT_EXIST){
        // traverse the terminal list, assign whichever terminal exist to be its head
        for (ii = 0; ii < TER_MAX; ii ++){
            if(ece391_multi_ter_status[(uint32_t)ii] == TER_EXIST){
                ece391_multi_ter_info[cur_ter_num].Parent_ter = ii;
                switch_terminal(TO_PARENT);
                return ;
            }
        }
        ERROR_MSG;
        printf("TERMINAL LOST HEAD TO RETURN");
        while(1);
    }
    // handle the destination terminal not exist
    if(next_terminal == TO_DESTI && ece391_multi_ter_status[(uint32_t)next_ter_number] == TER_NOT_EXIST){
        // TODO switch terminal and initiate to execute "shell"
        // ece391_multi_ter_info[(uint32_t)next_ter_number].Parent_ter = cur_ter_num; // assign the next_terminal's parent to be the current one
        /* TODO paging, cur_pid, */
        /*switch terminal video memory*/
        switch_terminal_video(cur_ter_num, next_ter_number);
        ece391_multi_ter_status[(uint32_t)next_ter_number] = TER_EXIST;
        clear();
        execute((void *)"shell");
        ece391_multi_ter_status[(uint32_t)next_ter_number] = TER_NOT_EXIST;
        switch_terminal(TO_PARENT);
        ERROR_MSG;
        printf("TERMINAL FAIL TO RETURN TO PARENT");
        while(1);
    }
    // handle the next_terminal that exists
    // TODO switch terminal and initiate to execute "shell"
    // ece391_multi_ter_info[(uint32_t)next_ter_number].Parent_ter = cur_ter_num;  // assign the next_terminal's parent to be the current one
    /* TODO paging, cur_pid, */
    /*switch terminal video memory*/
    switch_terminal_video(cur_ter_num, next_ter_number);
    /* update cur_pid */
    ece391_process_manager.curr_pid = ece391_multi_ter_info[(uint32_t)next_ter_number].PID_num;
    /*switch destination terminal process user memory*/
    switch_terminal_paging(ece391_process_manager.curr_pid);
    /* gives the notification */
    printf("[ATTENTION] SWITCH TO TERMINAL %d\n", (uint32_t)next_ter_number);
    /* ss */
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].SS_reg));
    /* esp */
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].ESP_reg));
    /* eflags */
    asm volatile("push  %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].EFLAGS_reg));
    /* cs */
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].CS_reg));
    /* eip*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].old_EIP_reg));
    /* fs*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].FS_reg));
    /* es*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].ES_reg));
    /* ds*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].DS_reg));
    /* eax*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].EAX_reg));
    /* ebp*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].EBP_reg));
    /* edi*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].EDI_reg));
    /* esi*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].ESI_reg));
    /* edx*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].EDX_reg));
    /* ecx*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].ECX_reg));
    /* ebx*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].EBX_reg));
    /* pop all the register just poped on the stack*/
    asm volatile("popal" : :);
    /* use iret to switch context */
    asm volatile("iret" : :);

}
