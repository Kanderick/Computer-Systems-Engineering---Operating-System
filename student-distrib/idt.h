#ifndef IDT_H
#define IDT_H

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

void idt_init(unsigned index, unsigned gateType);
void exception_DE();
void exception_DB();
void exception_NMI();
void exception_BP();
void exception_OF();
void exception_BR();
void exception_UD();
void exception_NM();
void exception_CSO();
void exception_TS();
void exception_NP();
void exception_SS();
void exception_GP();
void exception_PF();
void exception_MF();
void exception_AC();
void exception_MC();
void exception_XF();

#endif
