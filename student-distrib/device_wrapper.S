# device_wrapper.S - Set up the keyboard and rtc handler
# #include "multi_terminal.h"

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
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %ebp
    pushl %esi
    pushl %edi
    pushw %ss
    pushw %ds
    pushw %es
    pushw %fs
    pushw %gs
    pushl %esp              # push the register
    pushfl
    call pit_interrupt  # Pit IT may not come back. But in that circumstance, we don't need to restore it's original register value. See boot_new_shell()
    popfl
    popl %esp
    popw %gs
    popw %fs
    popw %es
    popw %ds
    popw %ss
    popl %edi
    popl %esi
    popl %ebp
    popl %edx
    popl %ecx
    popl %ebx           # pop the register
    iret
