
/* NOTE donot forget to uncomment  line 45 in idt.c file */

.text
.globl system_call_wrapper

system_call_wrapper:
    cmpl $1, %eax
    jl fail
    cmpl $10, %eax
    jg fail
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
    pushl %esp
    pushfl
    pushl %edx
    pushl %ecx
    pushl %ebx
    call *system_call_jump_table(, %eax, 4)
    addl $12, %esp
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
    popl %ebx
    iret
fail:
    movl $-1, %eax
    iret
system_call_jump_table:
    .long 0, halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn
