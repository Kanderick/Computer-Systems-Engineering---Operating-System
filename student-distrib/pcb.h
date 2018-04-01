typedef struct process_control_block {
    fa array;
    uint32_t parent_ebp;
    uint32_t parent_esp;
    uint8_t parent_pid;
    uint32_t ebp;
    uint32_t esp;
    uint8_t pid;
} pcb;
