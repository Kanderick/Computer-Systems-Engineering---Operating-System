#include "multi_terminal.h"
#include "system_call.h"
#include "types.h"
// current terminal number
uint8_t cur_ter_num;

// ece391 terminal info array
MULTI_TER_INFO_t ece391_multi_ter_info[TER_MAX];

// ece391 terminal status array
uint8_t ece391_multi_ter_status[TER_MAX];

// init multi-terminal
void multi_terminal_init(){
    uint8_t ii;
    for (ii = 0; ii < TER_MAX; ii++)
        ece391_multi_ter_status[ii] = TER_NOT_EXIST;
    cur_ter_num = 0;
    ece391_multi_ter_status[cur_ter_num] = TER_EXIST;
    ece391_multi_ter_info[cur_ter_num].Parent_ter = -1; // set Parent_ter to nonsense
}

// switch_terminal function
void switch_terminal(uint32_t next_terminal){
    int8_t next_ter_number, ii;

    // extract the terminal number to jump to
    if (next_terminal == TO_DESTI)
        next_ter_number = ece391_multi_ter_info[cur_ter_num].Dest_ter;
    else if (next_terminal == TO_PARENT)
        next_ter_number = ece391_multi_ter_info[cur_ter_num].Parent_ter;
    else
        return ;
    // handle invalid terminal number
    if(next_terminal<0 || next_terminal>2){
        ERROR_MSG;
        printf("INVALID TERMINAL NUMBER");
        while(1);
    }
    // handle the parent terminal not exist
    if(next_terminal == TO_PARENT && ece391_multi_ter_statusp[next_ter_number] == TER_NOT_EXIST){
        for (ii = 0; ii < TER_MAX; ii ++){
            if(ece391_multi_ter_status[ii] == TER_EXIST){
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
    if(next_terminal == TO_DESTI && ece391_multi_ter_statusp[next_ter_number] == TER_NOT_EXIST){
        // TODO switch terminal and initiate to execute "shell"
        ece391_multi_ter_info[next_ter_number].Parent_ter = cur_ter_num;
        /* TODO paging, cur_pid, */
        execute("shell");
        switch_terminal(TO_PARENT);
        ERROR_MSG;
        printf("TERMINAL FAIL TO RETURN TO PARENT");
        while(1);
    }
    // handle the destination that exists
    // TODO switch terminal and initiate to execute "shell"
    ece391_multi_ter_info[next_ter_number].Parent_ter = cur_ter_num;
    /* TODO paging, cur_pid, */
    /* ss */
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].SS));
    /* esp */
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].ESP));
    /* eflags */
    asm volatile("push  %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].EFLAGS));
    /* cs */
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].CS));
    /* eip*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].old_EIP));
    /* fs*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].FS));
    /* es*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].ES));
    /* ds*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].DS));
    /* eax*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].EAX));
    /* ebp*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].EBP));
    /* edi*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].EDI));
    /* esi*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].ESI));
    /* edx*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].EDX));
    /* ecx*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].ECX));
    /* ebx*/
    asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[next_terminal].EBX));


}
