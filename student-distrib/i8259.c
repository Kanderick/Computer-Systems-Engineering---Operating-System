/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */
#include "types.h"
#include "i8259.h"
#include "lib.h"

#define HAVE_SPIN_LOCK 0

/* Interrupt masks to determine which interrupts are enabled and disabled */
static uint8_t master_mask; /* IRQs 0-7  */
static uint8_t slave_mask;  /* IRQs 8-15 */


// IO_WAIT wait for an I/O operation to complete
// static inline void io_wait(void)
// {
//     /* TODO: This is probably fragile. */
//     asm volatile ( "jmp 1f\n\t"
//                    "1:jmp 2f\n\t"
//                    "2:" );
// }

/* Initialize the 8259 PIC */
void i8259_init(void) {
    // store mask
    outb(MASK_OFF, PIC1_DATA);
    outb(MASK_OFF, PIC2_DATA);
    // initialization PIC1
    outb(PIC_INIT_CODE, PIC1_COMMAND);    /* ICW1: select 8259A-1 init */
    outb(PIC1_MAP_OFFSET, PIC1_DATA);     /* ICW2: map IR0-7 to 0x20-0x27 */
    outb(SLAVE_MASK_, PIC1_DATA);         /* ICW3: load slave mask */
    outb(NORMAL_EOI, PIC1_DATA);          /* ICW4: master expect normal EOI */

    // initialization PIC2
    outb(PIC_INIT_CODE, PIC2_COMMAND);    /* ICW1: select 8259A-2 init */
    outb(PIC2_MAP_OFFSET, PIC2_DATA);     /* ICW2: map IR8-15 to 0x28-0x2f */
    outb(SLAVE_BIT_, PIC2_DATA);         /* ICW3: load slave bit */
    outb(NORMAL_EOI, PIC2_DATA);          /* ICW4: master expect normal EOI */
    // set ACK
    master_mask = MASK_OFF;
    slave_mask = MASK_OFF;
    enable_irq(SLAVE_BIT_);

}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    if (irq_num < 8) {
        master_mask &= ~(1 << irq_num);
        outb(master_mask, PIC1_DATA);
    }
    else {  // if irq received from PIC2
        irq_num -= IRQ_NUM_;
        slave_mask &= ~(1 << irq_num);
        outb(slave_mask, PIC2_DATA);
    }
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    // if irq received from PIC1
    if(irq_num < 8) {
        master_mask |= (1 << irq_num);
        outb(master_mask, PIC1_DATA);
    }
    else {  // if irq received from PIC2
        irq_num -= IRQ_NUM_;
        slave_mask |= (1 << irq_num);
        outb(slave_mask, PIC2_DATA);
    }
    // mask the corresponding bit to 1
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    uint8_t value;

    if(irq_num >= 8) {
        irq_num -= IRQ_NUM_;
        value = EOI | irq_num;
        outb(value, SLAVE_8259_PORT);
        outb((EOI | 0x02), MASTER_8259_PORT);
    }
    else {
        value = EOI | irq_num;
        // outb(master_mask, PIC1_DATA);
        outb(value, MASTER_8259_PORT);
    }
}
