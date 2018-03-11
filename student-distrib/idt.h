#ifndef IDT_H
#define IDT_H

#include "types.h"
#include "x86_desc.h"
#include "lib.h"

#define DE      0       /*Divide Error Exception*/
#define DB      1       /*Debug Exception*/
#define NMI     2       /*NMI Interrupt*/
#define BP      3       /*Breakpoint Exception*/
#define OF      4       /*Overflow Exception*/
#define BR      5       /*BOUND Range Exceeded Exception*/
#define UD      6       /*Invalid Opcode Exception*/
#define NM      7       /*Device Not Available Exception*/
#define DF      8       /*Double Fault Exception*/
#define CSO     9       /*Coprocessor Segment Overrun*/
#define TS      10      /*Invalid TSS Exception*/
#define NP      11      /*Segment Not Present*/
#define SS      12      /*Stack Fault Exception*/
#define GP      13      /*General Protection Exception*/
#define PF      14      /*Page-Fault Exception*/
#define MF      16      /*x87 FPU Floating-Point Error*/
#define AC      17      /*Alignment Check Exception*/
#define MC      18      /*Machine-Check Exception*/
#define XF      19      /*SIMD Floating-Point Exception*/
#define KB_INT  0x21    /*keyboard*/
#define RTC_INT 0x28    /*real time clock*/
#define SCV     0x80    /*system call vector*/
#define INT_GATE    0   /*interrupt gate descriptor*/
#define TRAP_GATE   1   /*trap gata descriptor*/
#define CALL_GATE   2   /*call gate descriptor*/
#define DPL_IE      0   /*Attempting  to  access  an  interrupt  or  exception
                        handler  through  an  interrupt  or  trap  gate from
                        virtual-8086 mode when the handler’s code segment DPL
                        is greater than 0.*/
#define DPL_SC      3   /*dpl of system call*/
                        /*Gate descriptors are in themselves “gates,” which
                        hold pointers to procedure entry points in code segments
                        (call, interrupt, and trap gates) */

void idt_init_exceptions(void);
void idt_init(unsigned index, unsigned gateType, void *handler);
void exception_DE(void);
void exception_DB(void);
void exception_NMI(void);
void exception_BP(void);
void exception_OF(void);
void exception_BR(void);
void exception_UD(void);
void exception_NM(void);
void exception_DF(void);
void exception_CSO(void);
void exception_TS(void);
void exception_NP(void);
void exception_SS(void);
void exception_GP(void);
void exception_PF(void);
void exception_MF(void);
void exception_AC(void);
void exception_MC(void);
void exception_XF(void);
void unkown_int(void);

extern void exception_DE_wrapper(void);
extern void exception_DB_wrapper(void);
extern void exception_NMI_wrapper(void);
extern void exception_BP_wrapper(void);
extern void exception_OF_wrapper(void);
extern void exception_BR_wrapper(void);
extern void exception_UD_wrapper(void);
extern void exception_NM_wrapper(void);
extern void exception_DF_wrapper(void);
extern void exception_CSO_wrapper(void);
extern void exception_TS_wrapper(void);
extern void exception_NP_wrapper(void);
extern void exception_SS_wrapper(void);
extern void exception_GP_wrapper(void);
extern void exception_PF_wrapper(void);
extern void exception_MF_wrapper(void);
extern void exception_AC_wrapper(void);
extern void exception_MC_wrapper(void);
extern void exception_XF_wrapper(void);
extern void unkown_int_wrapper(void);



#endif
