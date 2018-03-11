.globl exception_DE_wrapper
.globl exception_DB_wrapper
.globl exception_NMI_wrapper
.globl exception_BP_wrapper
.globl exception_OF_wrapper
.globl exception_BR_wrapper
.globl exception_UD_wrapper
.globl exception_NM_wrapper
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

exception_DE_wrapper:
    pushl %ebp
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    call exception_DE
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
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
    pushl %edi
    pushl %esi
    pushl %ebx
    pushl %edx
    pushl %ecx
    pushl %eax
    popl %ebp
    iret
