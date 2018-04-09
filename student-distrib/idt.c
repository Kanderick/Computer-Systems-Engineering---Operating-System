/* idt.c - Functions to initialize the IDT table
 */
#include "idt.h"
#include "x86_desc.h"
// include this only to initialze rtc's flag
#include "rtc.h"
/*
 * idt_init_exceptions
 *   DESCRIPTION: this function will be called in the kernel.c and
                  set up and initialize all the entry in the IDT
                  table
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: initialize all the entry with specific instruction
 */

void idt_init_exceptions(void){
    unsigned i;
    for (i = 0; i < 256; i ++) {            /*fill all the entry in the idt table with unkown interrupt*/
        idt_init(i, INT_GATE, &unkown_int_wrapper);
    }
    idt_init(DE, INT_GATE, &exception_DE_wrapper);         /*fill the exception part with each specific instruction*/
    idt_init(DB, INT_GATE, &exception_DB_wrapper);
    idt_init(NMI, INT_GATE, &exception_NMI_wrapper);
    idt_init(BP, INT_GATE, &exception_BP_wrapper);
    idt_init(OF, INT_GATE, &exception_OF_wrapper);
    idt_init(BR, INT_GATE, &exception_BR_wrapper);
    idt_init(UD, INT_GATE, &exception_UD_wrapper);
    idt_init(NM, INT_GATE, &exception_NM_wrapper);
    idt_init(DF, INT_GATE, &exception_DF_wrapper);
    idt_init(CSO, INT_GATE, &exception_CSO_wrapper);
    idt_init(TS, INT_GATE, &exception_TS_wrapper);
    idt_init(NP, INT_GATE, &exception_NP_wrapper);
    idt_init(SS, INT_GATE, &exception_SS_wrapper);
    idt_init(GP, INT_GATE, &exception_GP_wrapper);
    idt_init(PF, INT_GATE, &exception_PF_wrapper);
    idt_init(UN, INT_GATE, &unkown_int_wrapper);
    idt_init(MF, INT_GATE, &exception_MF_wrapper);
    idt_init(AC, INT_GATE, &exception_AC_wrapper);
    idt_init(MC, INT_GATE, &exception_MC_wrapper);
    idt_init(XF, INT_GATE, &exception_XF_wrapper);
    idt_init(KB_INT, INT_GATE, &keyboard_wrapper);      /*fill the keyboard interruption entry*/
    idt_init(RTC_INT, INT_GATE, &rtc_wrapper);          /*fill the rtc interruption entry*/
    idt_init(SCV, TRAP_GATE, &system_call_wrapper);
    // init rtc flag here
    rtcFlag = 1;
}

/*
 * idt_init
 *   DESCRIPTION: this function will be called in the kernel.c and
                  set up all the entry in the IDT table
 *   INPUTS: index -- the index for the entry that should be filled in
             gateType -- the type of the gate the entry should use
             handler -- the addr of the handler wrapper
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: initialize the specific entry
 */

void idt_init(unsigned index, unsigned gateType, void *handler) {
    idt_desc_t desc = idt[index];       /*get the position of the entry in the idt table*/
    SET_IDT_ENTRY(desc, handler);       /*set handler for this entry*/
    desc.seg_selector = KERNEL_CS;      /*Gate descriptors are in themselves “gates,” which
                                        hold pointers to procedure entry points in code segments
                                        (call, interrupt, and trap gates) */
    desc.size = 1;                      /*trap, call and task gate are 32 bits*/
    desc.present = 1;                   /*make it present*/
    switch(gateType) {                  /*fill up the reserved bits for each gate*/
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
            desc.dpl = DPL_SC;
            break;
        case CALL_GATE:
            desc.reserved4 = 0;
            desc.reserved3 = 0;
            desc.reserved2 = 0;
            desc.reserved1 = 1;
            desc.reserved0 = 0;
            break;
    }
    idt[index] = desc;          /*put back the entry to the idt table*/
}

/*
 * exception_DE
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of DE
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_DE(void) {
    cli();          /*clean the interrupt flag*/
    printf("Divide Error Exception\n");
}

/*
 * exception_DB
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of DB
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_DB(void) {
     cli();         /*clean the interrupt flag*/
    printf("Debug Exception\n");
}

/*
 * exception_NMI
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of NMI
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_NMI(void) {
     cli();         /*clean the interrupt flag*/
    printf("NMI Interrupt\n");
}

/*
 * exception_BP
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of BP
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_BP(void) {
     cli();         /*clean the interrupt flag*/
    printf("Breakpoint Exception\n");
}

/*
 * exception_OF
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of OF
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_OF(void) {
     cli();         /*clean the interrupt flag*/
    printf("Overflow Exception\n");
}

/*
 * exception_BR
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of BR
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_BR(void) {
     cli();         /*clean the interrupt flag*/
    printf("BOUND Range Exceeded Exception\n");
}

/*
 * exception_UD
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of UD
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_UD(void) {
     cli();         /*clean the interrupt flag*/
    printf("Invalid Opcode Exception\n");
}

/*
 * exception_NM
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of NM
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_NM(void) {
     cli();         /*clean the interrupt flag*/
    printf("Device Not Available Exception\n");
}

/*
 * exception_DF
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of DF
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_DF(void) {
     cli();         /*clean the interrupt flag*/
    printf("Double Fault Exception\n");
}

/*
 * exception_CSO
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of CSO
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_CSO(void) {
     cli();         /*clean the interrupt flag*/
    printf("Coprocessor Segment Overrun\n");
}

/*
 * exception_TS
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of TS
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_TS(void) {
     cli();         /*clean the interrupt flag*/
    printf("Invalid TSS Exception\n");
}

/*
 * exception_NP
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of NP
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_NP(void) {
    cli();          /*clean the interrupt flag*/
    printf("Segment Not Present\n");
}

/*
 * exception_SS
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of SS
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_SS(void) {
    cli();          /*clean the interrupt flag*/
    printf("Stack Fault Exception\n");
}

/*
 * exception_GP
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of GP
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_GP(void) {
    cli();          /*clean the interrupt flag*/
    printf("General Protection Exception\n");
    while(1);
}

/*
 * exception_PF
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of PF
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_PF(void) {
    cli();          /*clean the interrupt flag*/
    printf("Page-Fault Exception\n");
    while(1);
}

/*
 * exception_MF
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of MF
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_MF(void) {
    cli();          /*clean the interrupt flag*/
    printf("x87 FPU Floating-Point Error\n");
}

/*
 * exception_AC
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of AC
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_AC(void) {
    cli();          /*clean the interrupt flag*/
    printf("Alignment Check Exception\n");
}

/*
 * exception_MC
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of MC
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_MC(void) {
    cli();          /*clean the interrupt flag*/
    printf("Machine-Check Exception\n");
}

/*
 * exception_XF
 *   DESCRIPTION: this function will be called by the exception wrapper
                  to execute the instruction of XF
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void exception_XF(void) {
    cli();          /*clean the interrupt flag*/
    printf("SIMD Floating-Point Exception\n");
}

/*
 * unkown_int
 *   DESCRIPTION: this function will be called by the unkonwn interruption
                  wrapper to execute the instruction of DE
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: print the related exception message in the screen
 */

void unkown_int(void) {
    cli();          /*clean the interrupt flag*/
    printf("unkown interruption\n");
}
