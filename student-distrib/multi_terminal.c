#include "multi_terminal.h"
#include "system_call.h"
#include "types.h"
#include "lib.h"
#include "pcb.h"
#include "device.h"

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
}

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

        ece391_multi_ter_info[cur_exe_ter_num].ter_screen_x = getScreen_x();
        ece391_multi_ter_info[cur_exe_ter_num].ter_screen_y = getScreen_y();
        memcpy(ece391_multi_ter_info[cur_exe_ter_num].ter_buffer, getBuffer(), BUFF_SIZE);
        ece391_multi_ter_info[cur_exe_ter_num].ter_bufferIdx = getIdx();

        /* Step2 Switch to correct page, both video and user code */
        background_uservideo_paging(cur_ter_num, (cur_exe_ter_num + 1) % TER_MAX);

        /* ===== Now everthing is happening on next kernel stack ===== */
        cur_exe_ter_num = (cur_exe_ter_num + 1) % TER_MAX;

        /* Step3 If next shell is running */
        if (ece391_multi_ter_status[cur_exe_ter_num] == TER_EXIST) {
            /* Restore next shell's esp and ebp*/
            /* Restore all previously stored next shell's registers */
            /* Just like halt */
            setIdx(ece391_multi_ter_info[cur_exe_ter_num].ter_bufferIdx);
            memcpy(getBuffer(), ece391_multi_ter_info[cur_exe_ter_num].ter_buffer, BUFF_SIZE);
            setScreen_x(ece391_multi_ter_info[cur_exe_ter_num].ter_screen_x);
            setScreen_y(ece391_multi_ter_info[cur_exe_ter_num].ter_screen_y);

            ece391_process_manager.curr_pid = ece391_multi_ter_info[cur_exe_ter_num].ter_pid_num;
            switch_terminal_paging(ece391_process_manager.curr_pid);
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
            asm volatile("movl %0, %%ebp\n\t": :"g" (PCB_BASE_ADDR - (next_avaliable_pid_num - 1) * PCB_SEG_LENGTH));
            asm volatile("movl %0, %%esp\n\t": :"g" (PCB_BASE_ADDR - (next_avaliable_pid_num - 1) * PCB_SEG_LENGTH));
            sti();
            while (1) {
                printf("A new shell should be able to boot up shortly.\n");
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
    // while (1) {
    printf("A new shell should be able to boot up shortly.\n");
    ece391_multi_ter_status[cur_exe_ter_num] = TER_NOT_EXIST;
    return;
    // }
}
