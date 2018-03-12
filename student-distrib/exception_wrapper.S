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

exception_DE_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_DE
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_DB_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_DB
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_NMI_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_NMI
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_BP_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_BP
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_OF_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_OF
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_BR_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_BR
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_UD_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_UD
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_NM_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_NM
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_DF_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_CSO
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_CSO_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_CSO
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_TS_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_TS
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_NP_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_NP
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_SS_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_SS
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_GP_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_GP
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_PF_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_PF
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_MF_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_MF
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_AC_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_AC
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_MC_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_MC
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
exception_XF_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_XF
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
unkown_int_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call unkown_int
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
    popl %ebp
    iret
