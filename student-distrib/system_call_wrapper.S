
/* NOTE donot forget to uncomment  line 45 in idt.c file */

.text
.globl system_call_wrapper

system_call_wrapper:
    cmpl $0, %eax
    jl fail
    cmpl $9, %eax
    jg fail
    pushal
    pushfl
    pushl %ebx
    pushl %ecx
    pushl %edx
    call *system_call_jump_table(, %eax, 4)
    addl $12, %esp
    popfl
    popal
    iret
fail:
    movl $-1, %eax
    iret
system_call_jump_table:
    .long halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn
