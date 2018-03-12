#include "paging.h"

/* Declaration of Global Page Directory/Table */
pde_t page_directory[PAGE_DIRECTORY_SIZE] __attribute__((aligned (_4KB)));
pte_t page_table_0[PAGE_TABLE_SIZE] __attribute__((aligned (_4KB)));

/* Static function definition */
static inline unsigned long read_cr0(void);
static inline unsigned long read_cr4(void);
static inline void write_cr0(unsigned long val);
static inline void write_cr3(unsigned long val);
static inline void write_cr4(unsigned long val);
static inline void set_in_cr0 (unsigned long mask);
// static inline void clear_in_cr0 (unsigned long mask);
static inline void set_in_cr4 (unsigned long mask);
static inline void clear_in_cr4 (unsigned long mask);

/* read_cr0
 * Purpose	Read in CR0 register value
 * Inputs	None
 * Outputs	CR0 value
 * Side Effects None
 */
static inline unsigned long read_cr0(void) {
    unsigned long val;
    asm volatile("movl %%cr0,%0\n\t" :"=r" (val));
    return val;
}

/* read_cr4
 * Purpose	Read in CR4 register value
 * Inputs	None
 * Outputs	CR4 value
 * Side Effects None
 */
static inline unsigned long read_cr4(void) {
    unsigned long val;
    asm volatile("movl %%cr4,%0\n\t" :"=r" (val));
    return val;
}

/* write_cr0
 * Purpose	Set CR0 register value
 * Inputs	CR0 value
 * Outputs	None
 * Side Effects CR0 value changed
 */
static inline void write_cr0(unsigned long val) {
    asm volatile("movl %0,%%cr0": :"r" (val));
}

/* write_cr3
 * Purpose	Set CR3 register value
 * Inputs	CR3 value
 * Outputs	None
 * Side Effects CR3 value changed
 */
static inline void write_cr3(unsigned long val) {
    asm volatile("movl %0,%%cr3": :"r" (val));
}

/* write_cr4
 * Purpose	Set CR4 register value
 * Inputs	CR4 value
 * Outputs	None
 * Side Effects CR4 value changed
 */
static inline void write_cr4(unsigned long val) {
    asm volatile("movl %0,%%cr4": :"r" (val));
}

/* set_in_cr0
 * Purpose	Set CR0 register bit without change other bits
 * Inputs	Mask for bits want to set
 * Outputs	None
 * Side Effects CR0 value changed
 */
static inline void set_in_cr0(unsigned long mask) {
    unsigned long cr0;
    cr0 = read_cr0();
    cr0 |= mask;
    write_cr0(cr0);
}

// Does not use in this CP3.1, reserved for future use
/* clear_in_cr0
 * Purpose	Clear CR0 register bit without change other bits
 * Inputs	Mask for bits want to clear
 * Outputs	None
 * Side Effects CR0 value changed
 */
/*static inline void clear_in_cr0(unsigned long mask) {
    unsigned long cr0;
    cr0 = read_cr0();
    cr0 &= ~mask;
    write_cr0(cr0);
}*/

/* set_in_cr4
 * Purpose	Set CR4 register bit without change other bits
 * Inputs	Mask for bits want to set
 * Outputs	None
 * Side Effects CR4 value changed
 */
static inline void set_in_cr4(unsigned long mask) {
	unsigned long cr4;
	cr4 = read_cr4();
	cr4 |= mask;
	write_cr4(cr4);
}

/* clear_in_cr4
 * Purpose	Clear CR4 register bit without change other bits
 * Inputs	Mask for bits want to clear
 * Outputs	None
 * Side Effects CR4 value changed
 */
static inline void clear_in_cr4(unsigned long mask) {
	unsigned long cr4;
	cr4 = read_cr4();
	cr4 &= ~mask;
	write_cr4(cr4);
}

/* init_paging
 * Purpose	Turn on paging
 * Inputs	None
 * Outputs	None
 * Side Effects CR0,3,4 value changed, paging turned on
 */
void init_paging(void) {
	unsigned int i, j;
	/* Flush page directory */
    for (i = 0; i < PAGE_DIRECTORY_SIZE; i++) {
        page_directory[i] = 0x00000000;
    }

    /* Flush page table 0 */
    for (j = 0; j < PAGE_TABLE_SIZE; j++) {
        page_table_0[j] = 0x00000000;
    }

    /* Points to the page_table_0 */
    page_directory[0] = ((unsigned long)(page_table_0) & PTBA_MASK) | PRESENT_MASK | R_W_MASK;

    /* Points to the start of Video memory */
    page_table_0[VIDEO_VIRTUAL] = VIDEO_START | R_W_MASK | PRESENT_MASK;

    /* Kernel 4MB page, present, R/W on */
    page_directory[1] = KERNEL_START | PAGE_SIZE_MASK | R_W_MASK | PRESENT_MASK;

    /* Set CR4 Flags */
    clear_in_cr4(CR4_PAE_FLAG); /* PAE set to 0 */
	set_in_cr4(CR4_PSE_FLAG);   /* PSE set to 1 */

	/* Set CR3 */
    write_cr3((unsigned long)page_directory);   /* This instruction flushed the tlb */

    /* Set CR0 bit 31*/
    set_in_cr0(CR0_PG_FLAG);    /* Assert Page Table is turned on */
}
