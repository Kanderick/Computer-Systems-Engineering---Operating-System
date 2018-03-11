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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
    popl %ebp
    iret
