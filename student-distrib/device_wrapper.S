.text
.globl keyboard_wrapper
.globl rtc_wrapper

keyboard_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call keyboard_interrupt
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
rtc_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call rtc_interrupt
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
