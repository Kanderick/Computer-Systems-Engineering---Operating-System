#ifndef   MULTI_TER_H
#define   MULTI_TER_H
#include "paging.h"
#include "types.h"

#define TER_MAX         3       // has three terminal at most
#define TER_EXIST       1       // for ece391_multi_ter_status, indicating existance
#define TER_NOT_EXIST   0       // for ece391_multi_ter_status, indicating non-existance
#define TO_DESTI        0       // tell switch_terminal jump to destination terminal
#define TO_PARENT       1       // tell switch_terminal jump to parent terminal


// NOTE: DONOT change even the element order of the following structure
struct MULTI_TER_INFO {
    // runtime info, use this to jump back to old terminal
    uint32_t EBX;
    uint32_t ECX;
    uint32_t EDX;
    uint32_t ESI;
    uint32_t EDI;
    uint32_t EBP;
    uint32_t EAX;
    uint32_t DS;
    uint32_t ES;
    uint32_t FS;
    uint32_t old_EIP;
    uint32_t CS;
    uint32_t EFLAGS;
    uint32_t ESP;
    uint32_t SS;
    // if return to a terminal, use this pid to restore the cur_pid
    int8_t PID_num;
    // the parent terminal
    int8_t Parent_ter;
    // the destination terminal number
    int8_t Dest_ter;
} __attribute((packed)); // NOTE: MUST BE PACKED
typedef struct MULTI_TER_INFO MULTI_TER_INFO_t;

// current terminal number, should be either 0, 1, or 2
extern uint8_t cur_ter_num;

// ece391 terminal info array
extern MULTI_TER_INFO_t ece391_multi_ter_info[TER_MAX];

// ece391 terminal status array
extern uint8_t ece391_multi_ter_status[TER_MAX];

// init multi-terminal
void multi_terminal_init();

// switch_terminal function
void switch_terminal(uint32_t next_terminal);

#endif
