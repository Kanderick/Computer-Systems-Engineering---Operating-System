#include "idt.h"


//idt_init(DE, TRAP_GATE, exception_DE);
//idt_init(DB, TRAP_GATE, exception_DB);
//idt_init(NMI, TRAP_GATE, exception_NMI);
//idt_init(BP, TRAP_GATE, exception_BP);
//idt_init(OF, TRAP_GATE, exception_OF);
//idt_init(BR, TRAP_GATE, exception_BR);
//idt_init(UD, TRAP_GATE, exception_UD);
//idt_init(NM, TRAP_GATE, exception_NM);
//idt_init(CSO, TRAP_GATE, exception_CSO);
//idt_init(TS, TRAP_GATE, exception_TS);
//idt_init(NP, TRAP_GATE, exception_NP);
//idt_init(SS, TRAP_GATE, exception_SS);
//idt_init(GP, TRAP_GATE, exception_GP);
//idt_init(PF, TRAP_GATE, exception_PF);
//idt_init(MF, TRAP_GATE, exception_MF);
//idt_init(AC, TRAP_GATE, exception_AC);
//idt_init(MC, TRAP_GATE, exception_MC);
//idt_init(XF, TRAP_GATE, exception_XF);

/*IA-32 5.11 & 4.8*/
void idt_init(unsigned index, unsigned gateType, uint32_t handler) {
    idt_desc_t desc = idt[index];
    SET_IDT_ENTRY(desc, handler);
    desc.seg_selector = KERNEL_CS;
    desc.size = 1;
    desc.present = 1;
    switch(gateType) {
        case INT_GATE:
            desc.reserved4 = 0;
            desc.reserved3 = 0;
            desc.reserved2 = 1;
            desc.reserved1 = 1;
            desc.reserved0 = 0;
            desc.dpl = DPL_IE;
        case TRAP_GATE:
            desc.reserved4 = 0;
            desc.reserved3 = 1;
            desc.reserved2 = 1;
            desc.reserved1 = 1;
            desc.reserved0 = 0;
            desc.dpl = DPL_IE;
        case CALL_GATE:
            desc.reserved4 = 0;
            desc.reserved3 = 0;
            desc.reserved2 = 0;
            desc.reserved1 = 1;
            desc.reserved0 = 0;
            switch(index) {
                case SCV:
                    desc.dpl = DPL_SC;
            }
    }
    idt[index] = desc;
}

void exception_DE(void) {
    cli();
    printf("Divide Error Exception\n");
    while(1);
}

void exception_DB(void) {
    cli();
    printf("Debug Exception\n");
    while(1);
}

void exception_NMI(void) {
    cli();
    printf("NMI Interrupt\n");
    while(1);
}

void exception_BP(void) {
    cli();
    printf("Breakpoint Exception\n");
    while(1);
}

void exception_OF(void) {
    cli();
    printf("Overflow Exception\n");
    while(1);
}

void exception_BR(void) {
    cli();
    printf("BOUND Range Exceeded Exception\n");
    while(1);
}

void exception_UD(void) {
    cli();
    printf("Invalid Opcode Exception\n");
    while(1);
}

void exception_NM(void) {
    cli();
    printf("Device Not Available Exception\n");
    while(1);
}

void exception_DF(void) {
    cli();
    printf("Double Fault Exception\n");
    while(1);
}

void exception_CSO(void) {
    cli();
    printf("Coprocessor Segment Overrun\n");
    while(1);
}

void exception_TS(void) {
    cli();
    printf("Invalid TSS Exception\n");
    while(1);
}

void exception_NP(void) {
    cli();
    printf("Segment Not Present\n");
    while(1);
}

void exception_SS(void) {
    cli();
    printf("Stack Fault Exception\n");
    while(1);
}

void exception_GP(void) {
    cli();
    printf("General Protection Exception\n");
    while(1);
}

void exception_PF(void) {
    cli();
    printf("Page-Fault Exception\n");
    while(1);
}

void exception_MF(void) {
    cli();
    printf("x87 FPU Floating-Point Error\n");
    while(1);
}

void exception_AC(void) {
    cli();
    printf("Alignment Check Exception\n");
    while(1);
}

void exception_MC(void) {
    cli();
    printf("Machine-Check Exception\n");
    while(1);
}

void exception_XF(void) {
    cli();
    printf("SIMD Floating-Point Exception\n");
    while(1);
}
