
/* NOTE donot forget to uncomment  line 45 in idt.c file */

.text
.globl system_call_wrapper

system_call_wrapper:
    cmpl $1, %eax
    jl fail
    cmpl $10, %eax
    jg fail
    pushal
    pushfl
    pushl %edx
    pushl %ecx
    pushl %ebx
    call *system_call_jump_table(, %eax, 4)
    addl $12, %esp
    popfl
    popal
    iret
fail:
    movl $-1, %eax
    iret
system_call_jump_table:
    .long 0, halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn
