# exception_wrapper.S - Set up all the exception handler wrapper

.text
.globl exception_DE_wrapper
.globl exception_DB_wrapper
.globl exception_NMI_wrapper
.globl exception_BP_wrapper
.globl exception_OF_wrapper
.globl exception_BR_wrapper
.globl exception_UD_wrapper
.globl exception_NM_wrapper
.globl exception_DF_wrapper
.globl exception_CSO_wrapper
.globl exception_TS_wrapper
.globl exception_NP_wrapper
.globl exception_SS_wrapper
.globl exception_GP_wrapper
.globl exception_PF_wrapper
.globl exception_MF_wrapper
.globl exception_AC_wrapper
.globl exception_MC_wrapper
.globl exception_XF_wrapper
.globl unkown_int_wrapper

# wrappers to call the related functions to halt when exception or interruption happens

exception_DE_wrapper:
    pushal
    call exception_DE
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_DB_wrapper:
    pushal
    call exception_DB
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_NMI_wrapper:
    pushal
    call exception_NMI
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_BP_wrapper:
    pushal
    call exception_BP
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_OF_wrapper:
    pushal
    call exception_OF
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_BR_wrapper:
    pushal
    call exception_BR
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_UD_wrapper:
    pushal
    call exception_UD
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_NM_wrapper:
    pushal
    call exception_NM
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_DF_wrapper:
    pushal
    call exception_CSO
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_CSO_wrapper:
    pushal
    call exception_CSO
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_TS_wrapper:
    pushal
    call exception_TS
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_NP_wrapper:
    pushal
    call exception_NP
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_SS_wrapper:
    pushal
    call exception_SS
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_GP_wrapper:
    pushal
    call exception_GP
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_PF_wrapper:
    pushal
    call exception_PF
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_MF_wrapper:
    pushal
    call exception_MF
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_AC_wrapper:
    pushal
    call exception_AC
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_MC_wrapper:
    pushal
    call exception_MC
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

exception_XF_wrapper:
    pushal
    call exception_XF
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret

unkown_int_wrapper:
    pushal
    call unkown_int
    popal
    movl $256, %ebx
    pushal
    pushl %ebx
    call halt
    popl %ebx
    popal
    iret
