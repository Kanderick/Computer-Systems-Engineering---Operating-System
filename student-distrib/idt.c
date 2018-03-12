#include "idt.h"

void idt_init_exceptions(void){
    unsigned i;
    for(i = 0; i < 256; i ++) {
        idt_init(i, INT_GATE, &unkown_int_wrapper);
    }
    idt_init(DE, TRAP_GATE, &exception_DE_wrapper);
    idt_init(DB, TRAP_GATE, &exception_DB_wrapper);
    idt_init(NMI, INT_GATE, &exception_NMI_wrapper);
    idt_init(BP, TRAP_GATE, &exception_BP_wrapper);
    idt_init(OF, TRAP_GATE, &exception_OF_wrapper);
    idt_init(BR, TRAP_GATE, &exception_BR_wrapper);
    idt_init(UD, TRAP_GATE, &exception_UD_wrapper);
    idt_init(NM, TRAP_GATE, &exception_NM_wrapper);
    idt_init(DF, TRAP_GATE, &exception_DF_wrapper);
    idt_init(CSO, TRAP_GATE, &exception_CSO_wrapper);
    idt_init(TS, TRAP_GATE, &exception_TS_wrapper);
    idt_init(NP, TRAP_GATE, &exception_NP_wrapper);
    idt_init(SS, TRAP_GATE, &exception_SS_wrapper);
    idt_init(GP, TRAP_GATE, &exception_GP_wrapper);
    idt_init(PF, TRAP_GATE, &exception_PF_wrapper);
    idt_init(UN, TRAP_GATE, &unkown_int_wrapper);
    idt_init(MF, TRAP_GATE, &exception_MF_wrapper);
    idt_init(AC, TRAP_GATE, &exception_AC_wrapper);
    idt_init(MC, TRAP_GATE, &exception_MC_wrapper);
    idt_init(XF, TRAP_GATE, &exception_XF_wrapper);
    idt_init(KB_INT, INT_GATE, &keyboard_wrapper);
    idt_init(RTC_INT, INT_GATE, &rtc_wrapper);
}
/*IA-32 5.11 & 4.8*/
void idt_init(unsigned index, unsigned gateType, void *handler) {
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
            break;
        case TRAP_GATE:
            desc.reserved4 = 0;
            desc.reserved3 = 1;
            desc.reserved2 = 1;
            desc.reserved1 = 1;
            desc.reserved0 = 0;
            desc.dpl = DPL_IE;
            break;
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
            break;
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

void unkown_int(void) {
    cli();
   printf("unkown interruption\n");
    while(1);
}
