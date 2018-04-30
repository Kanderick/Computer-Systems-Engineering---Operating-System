#include "multi_terminal.h"
#include "system_call.h"
#include "types.h"
#include "lib.h"
#include "pcb.h"
#include "device.h"

// // terminal busy flag
// uint8_t ter_flag;

// current terminal number
uint8_t cur_ter_num;

// for scheduler, the current terminal being executed
uint8_t cur_exe_ter_num;

// ece391 terminal info array
multi_ter_info_t ece391_multi_ter_info[TER_MAX];

// ece391 terminal status array
uint8_t ece391_multi_ter_status[TER_MAX];

// init multi-terminal
void multi_terminal_init(){
    uint8_t ii, i;
    for (ii = 0; ii < TER_MAX; ii++) {
        ece391_multi_ter_status[ii] = TER_NOT_EXIST;
        ece391_multi_ter_info[ii].ter_screen_x = 0;
        ece391_multi_ter_info[ii].ter_screen_y = 0;
        ece391_multi_ter_info[ii].ter_bufferIdx = 0;
        // ece391_multi_ter_info[ii].PID_num = -1;
        ece391_multi_ter_info[ii].ter_pid_num = -1;
        for (i = 0; i < TER_BUFFER_LEN + 1; i++)
            ece391_multi_ter_info[ii].ter_buffer[i] = '\0'; // +1 since we need to detect ENTER after filled
    }
    // open terminal 0
    cur_ter_num = 0;
    cur_exe_ter_num = 0;
    // ece391_multi_ter_status[cur_ter_num] = TER_EXIST;
    // ece391_multi_ter_info[cur_ter_num].Parent_ter = -1; // set Parent_ter to nonsense
    // ter_flag = TER_NOT_BUSY;
}

// switch_context function
// exe current terminal number, if halt, exe a new shell
// this function should always be called with TO_DESTI
// void switch_context(uint32_t next_terminal) {
//     int8_t next_ter_number;
//     int i;
//     // extract the terminal number to jump to
//     if (next_terminal == TO_DESTI) {
//         next_ter_number = ece391_multi_ter_info[(uint32_t)cur_ter_num].Dest_ter;
//     }
//     else if (next_terminal == TO_PARENT) {
//         ERROR_MSG;
//         printf("ASK SCHEDULER TO FIND PARENT TERMINAL\n");
//         while(1);
//     }
//     else {
//         ERROR_MSG;
//         printf("UNKNOWN COMMAND\n");
//         return;
//     }
//     // handle invalid terminal number
//     if (next_terminal < 0 || next_terminal > 2) {
//         ERROR_MSG;
//         printf("INVALID TERMINAL NUMBER");
//         while(1);
//     }
//     // handle the destination terminal not exist
//     if (ece391_multi_ter_status[(uint32_t)next_ter_number] == TER_NOT_EXIST) {
//         ece391_multi_ter_status[(uint32_t)next_ter_number] = TER_EXIST;
//         /*change the current terminal to the next one */
//         // make new shell's parent be -1
//         ece391_process_manager.curr_pid = -1;
//         // clear scr
//         clearScreen();
//         // init terminal buffer
//         ece391_multi_ter_info[(uint32_t)next_ter_number].ter_bufferIdx = TER_NOT_EXIST;
//         ece391_multi_ter_info[(uint32_t)next_ter_number].PID_num = -1;
//         for (i = 0; i < TER_BUFFER_LEN+1; i ++)
//             ece391_multi_ter_info[(uint32_t)next_ter_number].ter_buffer[i] = '\0'; // +1 since we need to detect ENTER after filled
//         sti();
//         execute((void *)"shell"); // QUESTION may be interrupted
//         ERROR_MSG;
//         printf("TERMINAL FAIL TO RETURN TO PARENT");
//         while(1);
//     }
//     // handle the next_terminal that exists
//     // TODO switch terminal and initiate to execute "shell"
//     // ece391_multi_ter_info[(uint32_t)next_ter_number].Parent_ter = cur_ter_num;  // assign the next_terminal's parent to be the current one
//     /* TODO paging, cur_pid, */
//     /*switch terminal video memory*/
//     //switch_terminal_video(cur_ter_num, next_ter_number);
//     //
//
//     /* update cur_pid */
//     ece391_process_manager.curr_pid = ece391_multi_ter_info[(uint32_t)next_ter_number].PID_num;
//     /*switch destination terminal process user memory*/
//     switch_terminal_paging(ece391_process_manager.curr_pid);
//     /* gives the notification */
//
//     tss.esp0 = ece391_process_manager.process_position[(ece391_process_manager.curr_pid) - 1]->esp;
//     tss.ss0 = KERNEL_DS;
//
//     /* ss */
//     asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[(uint32_t)next_ter_number].SS_reg));
//     /* esp */
//     asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[(uint32_t)next_ter_number].ESP_reg));
//     /* eflags */
//     asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[(uint32_t)next_ter_number].EFLAGS_reg));
//     /* cs */
//     asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[(uint32_t)next_ter_number].CS_reg));
//     /* eip*/
//     asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[(uint32_t)next_ter_number].old_EIP_reg));
//     /* fs*/
//     asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[(uint32_t)next_ter_number].FS_reg));
//     /* es*/
//     asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[(uint32_t)next_ter_number].ES_reg));
//     /* ds*/
//     asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[(uint32_t)next_ter_number].DS_reg));
//     /* eax*/
//     asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[(uint32_t)next_ter_number].EAX_reg));
//     /* ebp*/
//     asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[(uint32_t)next_ter_number].EBP_reg));
//     /* edi*/
//     asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[(uint32_t)next_ter_number].EDI_reg));
//     /* esi*/
//     asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[(uint32_t)next_ter_number].ESI_reg));
//     /* edx*/
//     asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[(uint32_t)next_ter_number].EDX_reg));
//     /* ecx*/
//     asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[(uint32_t)next_ter_number].ECX_reg));
//     /* ebx*/
//     asm volatile("pushl %0\n\t" : :"g" (ece391_multi_ter_info[(uint32_t)next_ter_number].EBX_reg));
//     sti();
//     /* pop all the register just poped on the stack*/
//     //asm volatile("popal" : :);
//     asm volatile("popl %%ebx" : :);
//     asm volatile("popl %%ecx" : :);
//     asm volatile("popl %%edx" : :);
//     asm volatile("popl %%esi" : :);
//     asm volatile("popl %%edi" : :);
//     asm volatile("popl %%ebp" : :);
//     asm volatile("popl %%eax" : :);
//     asm volatile("popw %%ds" : :);
//     asm volatile("popw %%ds" : :);
//     asm volatile("popw %%es" : :);
//     asm volatile("popw %%es" : :);
//     asm volatile("popw %%fs" : :);
//     asm volatile("popw %%fs" : :);
//
//     /* use iret to switch context */
//     asm volatile("iret" : :);
//
// }

void scheduling() {
    static int8_t next_avaliable_pid_num = -1;
    // static pcb_t* current_pcb = NULL;
    next_avaliable_pid_num = next_avaliable_pid();
    if (next_avaliable_pid_num <= 0) {
        ERROR_MSG;
        while (1);
    }
    /* If current terminal is not running a shell, we boot up a new shell */
    if (ece391_multi_ter_status[cur_exe_ter_num] == TER_NOT_EXIST) {
        boot_new_shell();
        ERROR_MSG;
        return; // Easier to understand, but will actually never reach here
    }
    /* If current terminal is running a shell, we need to switch to next terminal */
    else {
        /* Step1 Save current shell's esp and ebp for easy restore */
        cli();
        asm volatile("movl %%esp, %0\n\t" :"=r" (ece391_multi_ter_info[cur_exe_ter_num].esp));
        asm volatile("movl %%ebp, %0\n\t" :"=r" (ece391_multi_ter_info[cur_exe_ter_num].ebp));
        ece391_multi_ter_info[cur_exe_ter_num].tss_esp0 = tss.esp0;
        ece391_multi_ter_info[cur_exe_ter_num].tss_ss0 = tss.ss0;
        ece391_multi_ter_info[cur_exe_ter_num].ter_pid_num = ece391_process_manager.curr_pid;

        /* Step2 Switch to correct page, both video and user code */
        background_uservideo_paging(cur_ter_num, (cur_exe_ter_num + 1) % TER_MAX);

        /* ===== Now everthing is happening on next kernel stack ===== */
        cur_exe_ter_num = (cur_exe_ter_num + 1) % TER_MAX;

        /* Step3 If next shell is running */
        if (ece391_multi_ter_status[cur_exe_ter_num] == TER_EXIST) {
            /* Restore next shell's esp and ebp*/
            /* Restore all previously stored next shell's registers */
            /* Just like halt */
            ece391_process_manager.curr_pid = ece391_multi_ter_info[cur_exe_ter_num].ter_pid_num;
            tss.esp0 = ece391_multi_ter_info[cur_exe_ter_num].tss_esp0;
            tss.ss0 = ece391_multi_ter_info[cur_exe_ter_num].tss_ss0;
            asm volatile("movl %0, %%ebp\n\t": :"g" (ece391_multi_ter_info[cur_exe_ter_num].ebp));
            asm volatile("movl %0, %%esp\n\t": :"g" (ece391_multi_ter_info[cur_exe_ter_num].esp));
            sti();
            return;
        }
        /* If next shell is not running, just wait */
        else {
            /* Change ebp / esp to the base of new page (context switch) */
            // current_pcb = ece391_process_manager.process_position[(uint8_t)next_avaliable_pid_num - 1];
            // PCB_BASE_ADDR - (next_avaliable_pid_num - 1) * PCB_SEG_LENGTH;
            asm volatile("movl %0, %%ebp\n\t": :"g" (PCB_BASE_ADDR - (next_avaliable_pid_num - 1) * PCB_SEG_LENGTH));
            asm volatile("movl %0, %%esp\n\t": :"g" (PCB_BASE_ADDR - (next_avaliable_pid_num - 1) * PCB_SEG_LENGTH));
            sti();
            while (1) {
                // printf("A new shell should be able to boot up shortly.\n");
            }
            ERROR_MSG;
            return; // Easier to understand, but will actually never reach here
        }
    }
}

/* In this case, a new shell will be created on the correct pcb, stored IT registers will be flushed. See device_wrapper.S:pit_wrapper(). */
void boot_new_shell() {
    int i;
    cli();
    clearScreen();
    ece391_process_manager.curr_pid = -1;
    ece391_multi_ter_info[cur_exe_ter_num].ter_screen_x = 0;
    ece391_multi_ter_info[cur_exe_ter_num].ter_screen_y = 0;
    ece391_multi_ter_info[cur_exe_ter_num].ter_bufferIdx = 0;
    ece391_multi_ter_info[cur_exe_ter_num].ter_pid_num = next_avaliable_pid();
    for (i = 0; i < TER_BUFFER_LEN + 1; i++)
        ece391_multi_ter_info[cur_exe_ter_num].ter_buffer[i] = '\0';
    ece391_multi_ter_status[cur_exe_ter_num] = TER_EXIST;
    sti();
    execute((void *)"shell");
    WARNING_MSG;
    printf("If not exit intentionally, this is an ERROR.\n");
    while (1) {
        printf("A new shell should be able to boot up shortly.\n");
    }
}

// exe_ter_num is the old terminal Number
// cur_exe_ter_num have been updated for the this turn
// void context_switch(int exe_ter_num) {
//     //switch uservideo mapping
//     background_uservideo_paging(exe_ter_num, cur_exe_ter_num);
//     //set current destination terminal number
//     ece391_multi_ter_info[exe_ter_num].Dest_ter = cur_exe_ter_num;
//     // set current terminal's parent to the old terminal number
//     ece391_multi_ter_info[cur_exe_ter_num].Parent_ter = exe_ter_num;
//     // store the old terminal number's pid number to info table
//     ece391_multi_ter_info[exe_ter_num].PID_num = ece391_process_manager.curr_pid;
// }
