/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */
#include "types.h"
#include "i8259.h"
#include "lib.h"

#define HAVE_SPIN_LOCK 0

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */
#if (HAVE_SPIN_LOCK == 1)
static spinlock_t i8259A_lock=SPIN_LOCK_UNLOCKED;   /*PIC's  spinlock*/
#endif

// IO_WAIT wait for an I/O operation to complete 
static inline void io_wait(void)
{
    /* TODO: This is probably fragile. */
    asm volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}

/* Initialize the 8259 PIC */
void i8259_init(void) {
    unsigned char MASK1, MASK2;
#if (HAVE_SPIN_LOCK == 1)
    unsigned long flags;
    /* require the lock */
    spin_lock_irqsave(&i8259A_lock, flags);
#endif
    // store mask
    MASK1=inb(PIC1_DATA);
    MASK2=inb(PIC2_DATA);
    outb(MASK_OFF, PIC1_DATA);
    outb(MASK_OFF, PIC2_DATA);
    // initialization PIC1
    outb(PIC_INIT_CODE, PIC1_COMMAND);    /* ICW1: select 8259A-1 init */
    io_wait();
    outb(PIC1_MAP_OFFSET, PIC1_DATA);     /* ICW2: map IR0-7 to 0x20-0x27 */
    io_wait();
    outb(SLAVE_MASK_, PIC1_DATA);         /* ICW3: load slave mask */
    io_wait();
    outb(NORMAL_EOI, PIC1_DATA);          /* ICW4: master expect normal EOI */
    io_wait();
    // initialization PIC2
    outb(PIC_INIT_CODE, PIC2_COMMAND);    /* ICW1: select 8259A-2 init */
    io_wait();
    outb(PIC2_MAP_OFFSET, PIC2_DATA);     /* ICW2: map IR8-15 to 0x28-0x2f */
    io_wait();
    outb(SLAVE_BIT_, PIC2_DATA);         /* ICW3: load slave bit */
    io_wait();
    outb(NORMAL_EOI, PIC2_DATA);          /* ICW4: master expect normal EOI */
    io_wait();
    // set ACK

    // restore masks
    outb(MASK1, PIC1_DATA);
    outb(MASK2, PIC2_DATA);
    // mask KEYBOARD
    disable_irq(KEYBOARD_IRQ);
    // mask RTC
    disable_irq(RTC_IRQ);
#if (HAVE_SPIN_LOCK == 1)
    spin_unlock_irqrestore(&i8259A_lock, flags);
#endif
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    uint16_t port;
    uint8_t value;
    // if irq received from PIC1
    if(irq_num<8){
        port = PIC1_DATA;
    }   else {  // if irq received from PIC2
        port = PIC2_DATA;
        irq_num -= IRQ_NUM_;
    }
    // unmask the corresponding bit to 0
    value = inb(port) & ~(1<<irq_num);
    outb(value, port);
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    uint16_t port;
    uint8_t value;
    // if irq received from PIC1
    if(irq_num<8){
        port = PIC1_DATA;
    }   else {  // if irq received from PIC2
        port = PIC2_DATA;
        irq_num -= IRQ_NUM_;
    }
    // mask the corresponding bit to 1
    value = inb(port) | (1<<irq_num);
    outb(value, port);
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    if(irq_num >= 8)
        outb(PIC2_COMMAND, EOI);
    outb(PIC1_COMMAND, EOI);
}
