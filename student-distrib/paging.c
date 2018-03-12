#include "paging.h"

pde_t page_directory[PAGE_DIRECTORY_SIZE];
pte_t page_table_0[PAGE_TABLE_SIZE] __attribute__((aligned (_4KB)));

static unsigned long read_cr0(void);
// static unsigned long read_cr3(void);
static unsigned long read_cr4(void);
static void write_cr0(unsigned long val);
static void write_cr3(unsigned long val);
static void write_cr4(unsigned long val);
static void set_in_cr0 (unsigned long mask);
// static void clear_in_cr0 (unsigned long mask);
static void set_in_cr4 (unsigned long mask);
static void clear_in_cr4 (unsigned long mask);

static unsigned long read_cr0(void) {
    unsigned long val;
    asm volatile("movl %%cr0,%0\n\t" :"=r" (val));
    return val;
}

// Does not use in this MP Checkpoint, reserved for future use
// static unsigned long read_cr3(void) {
//     unsigned long val;
//     asm volatile("movl %%cr3,%0\n\t" :"=r" (val));
//     return val;
// }

static unsigned long read_cr4(void) {
    unsigned long val;
    asm volatile("movl %%cr4,%0\n\t" :"=r" (val));
    return val;
}

static void write_cr0(unsigned long val) {
    asm volatile("movl %0,%%cr0": :"r" (val));
}

static void write_cr3(unsigned long val) {
    asm volatile("movl %0,%%cr3": :"r" (val));
}

static void write_cr4(unsigned long val) {
    asm volatile("movl %0,%%cr4": :"r" (val));
}

static void set_in_cr0(unsigned long mask) {
    unsigned long cr0;
    cr0 = read_cr0();
    cr0 |= mask;
    write_cr0(cr0);
}

// Does not use in this MP Checkpoint, reserved for future use
// static void clear_in_cr0(unsigned long mask) {
//     unsigned long cr0;
//     cr0 = read_cr0();
//     cr0 &= ~mask;
//     write_cr0(cr0);
// }

static void set_in_cr4(unsigned long mask) {
	unsigned long cr4;
	cr4 = read_cr4();
	cr4 |= mask;
	write_cr4(cr4);
}

static void clear_in_cr4(unsigned long mask) {
	unsigned long cr4;
	cr4 = read_cr4();
	cr4 &= ~mask;
	write_cr4(cr4);
}

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
    page_table_0[0xB8] = VIDEO_START | R_W_MASK | PRESENT_MASK;

    /* Kernel 4MB page, present, R/W on */
    page_directory[1] = KERNEL_START | PAGE_SIZE_MASK | R_W_MASK | PRESENT_MASK;

    /* Set CR4 Flags */
    clear_in_cr4(CR4_PAE_FLAG);
	set_in_cr4(CR4_PSE_FLAG);

	/* Set CR3 */
    write_cr3((unsigned long)page_directory); // This instruction actullly flushed the tlb

    /* Set CR0 bit 31*/
    set_in_cr0(CR0_PG_FLAG);
}
