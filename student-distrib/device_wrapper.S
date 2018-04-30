# device_wrapper.S - Set up the keyboard and rtc handler
# #include "multi_terminal.h"

.data
        # use to load runtime info to old
        # Useful offset constants for accessing members of a
        # struct MULTI_TER_INFO_t structure
        EBX_P = 0
        ECX_P = 4
        EDX_P = 8
        ESI_P = 12
        EDI_P = 16
        EBP_P = 20
        EAX_P = 24
        DS_P = 28
        ES_P = 32
        FS_P = 36
        old_EIP_P = 40
        CS_P = 44
        EFLAGS_P = 48
        ESP_P = 52
        SS_P = 56
.text
.globl keyboard_wrapper
.globl rtc_wrapper
.globl pit_wrapper


# the keyboard and rtc wrapper that will call their related handler
keyboard_wrapper:
    pushal
    call keyboard_interrupt
    popal
    iret

rtc_wrapper:
    pushal
    call rtc_interrupt
    popal
    iret

pit_wrapper:
    pushal
    call pit_interrupt
    popal
    iret
