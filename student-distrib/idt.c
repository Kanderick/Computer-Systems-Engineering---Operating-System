#include "idt.h"
#include "x86_desc.h"

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
idt_init(DE, TRAP_GATE);
idt_init(DB, TRAP_GATE);
idt_init(NMI, TRAP_GATE);
idt_init(BP, TRAP_GATE);
idt_init(OF, TRAP_GATE);
idt_init(BR, TRAP_GATE);
idt_init(UD, TRAP_GATE);
idt_init(NM, TRAP_GATE);
idt_init(CSO, TRAP_GATE);
idt_init(TS, TRAP_GATE);
idt_init(NP, TRAP_GATE);
idt_init(SS, TRAP_GATE);
idt_init(GP, TRAP_GATE);
idt_init(PF, TRAP_GATE);
idt_init(MF, TRAP_GATE);
idt_init(AC, TRAP_GATE);
idt_init(MC, TRAP_GATE);
idt_init(XF, TRAP_GATE);
/*IA-32 5.11 & 4.8*/
void idt_init(unsigned index, unsigned gateType) {
    idt_desc_t temp = idt[index];
    desc.seg_selector = KERNEL_CS;
    desc.reserved4 = 0;
    desc.size = 1;
    desc.present = 1;
    switch(gateType) {
        case INT_GATE:
            desc.reserved3 = 1;
            desc.reserved2 = 1;
            desc.reserved1 = 0;
            desc.reserved0 = 1;
            desc.dpl = DPL_IE;
        case TRAP_GATE:
            desc.reserved3 = 1;
            desc.reserved2 = 1;
            desc.reserved1 = 1;
            desc.reserved0 = 1;
            desc.dpl = DPL_IE;
        case CALL_GATE:
            desc.reserved3 = 1;
            desc.reserved2 = 0;
            desc.reserved1 = 0;
            desc.reserved0 = 0;
            /*not sure about dpl of call gate*/
    }
    idt[index] = desc;
}
