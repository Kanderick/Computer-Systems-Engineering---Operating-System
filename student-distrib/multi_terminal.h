#ifndef   MULTI_TER_H
#define   MULTI_TER_H
#include "paging.h"
#include "types.h"

#define TER_MAX         3       // has three terminal at most
#define TER_EXIST       1       // for ece391_multi_ter_status, indicating existance
#define TER_NOT_EXIST   0       // for ece391_multi_ter_status, indicating non-existance
#define TO_DESTI        0       // tell switch_terminal jump to destination terminal
#define TO_PARENT       1       // tell switch_terminal jump to parent terminal
#define TER_BUFFER_LEN  128     // for restore terminal buffer
#define TER_BUSY        0
#define TER_NOT_BUSY    1
// NOTE: DONOT change even the element order of the following structure
struct multi_ter_info {
    // runtime info, use this to jump back to old terminal
    // uint32_t EBX_reg;
    // uint32_t ECX_reg;
    // uint32_t EDX_reg;
    // uint32_t ESI_reg;
    // uint32_t EDI_reg;
    // uint32_t EBP_reg;
    // uint32_t EAX_reg;
    // uint32_t DS_reg;
    // uint32_t ES_reg;
    // uint32_t FS_reg;
    // uint32_t old_EIP_reg;
    // uint32_t CS_reg;
    // uint32_t EFLAGS_reg;
    // uint32_t ESP_reg;
    // uint32_t SS_reg;

    // for user runtime
    // cursor information

    // Actually not used, just don't delete
    // int executeFlag;

    // if return to a terminal, use this pid to restore the cur_pid
    // int8_t PID_num;
    // the parent terminal
    // int8_t Parent_ter;
    // the destination terminal number
    // int8_t Dest_ter;



    int ter_screen_x;
    int ter_screen_y;
    int ter_bufferIdx;
    uint8_t ter_buffer[TER_BUFFER_LEN+1];
    int8_t ter_pid_num;

    uint32_t tss_esp0;
    uint32_t tss_ss0;
    uint32_t esp;
    uint32_t ebp;
} __attribute((packed)); // NOTE: MUST BE PACKED
typedef struct multi_ter_info multi_ter_info_t;

// terminal busy flag
// extern uint8_t ter_flag;

// current terminal number, should be either 0, 1, or 2
extern uint8_t cur_ter_num;

// for scheduler, the current terminal being executed
extern uint8_t cur_exe_ter_num;

// ece391 terminal info array
extern multi_ter_info_t ece391_multi_ter_info[TER_MAX];

// ece391 terminal status array
extern uint8_t ece391_multi_ter_status[TER_MAX];

// init multi-terminal
void multi_terminal_init();

void scheduling();

inline void boot_new_shell();

inline void context_switch(int exe_ter_num);

// switch_terminal function
// void switch_terminal(uint32_t next_terminal);

void switch_context(uint32_t next_terminal);

#endif
