/* paging.h - Initialize paging
 * vim:ts=4 noexpandtab
 */
#ifndef PAGE_H
#define PAGE_H

#include "lib.h"
#include "types.h"

/* Macros regards size */
#define PAGE_DIRECTORY_SIZE     1024
#define PAGE_TABLE_SIZE         1024
#define _4KB                    4096        /* 4KB = 4096Bytes, 2^12*/
#define _4MB                    4194304     /* 4MB = 1024*4096Bytes, 2^22*/

/* Mask for Paging Directory/Table entries */
#define PRESENT_MASK            0x00000001  /* Clear present will clear all other bits */
#define R_W_MASK                0x00000002  /* Need to turn on read/write */
#define U_S_MASK                0x00000004
#define PWT_MASK                0x00000008
#define PCD_MASK                0x00000010
#define ACCESSED_MASK           0x00000020  /* This bit will be set after access */
#define DIRTY_MASK              0x00000040  /* This bit will be set after content change */
#define PAGE_SIZE_MASK          0x00000080  /* 4KB page when 0, 4MB page when 1 */
#define AVAILABLE_MASK          0x00000E00
#define PTBA_MASK               0xFFFFF000  /* Page Table Base Addr, in Directory */
#define PBA_4K_MASK             0xFFFFF000  /* 4KB Page Base Addr, in Table */
#define PBA_4M_MASK             0xFFC00000  /* 4MB Page Base Addr, in Directory */

/* Kernel and Video addr */
#define KERNEL_START            0x00400000
#define VIDEO_START             0x000B8000
#define VIDEO_VIRTUAL           0xB8

/* Control Registers' Flag Masks */
#define CR0_PG_FLAG             0x80000000
#define CR4_PSE_FLAG            0x00000010
#define CR4_PAE_FLAG            0x00000020
#define CR4_PGE_FLAG            0x00000080

/* Page Directory/Table Entry variable type */
typedef unsigned int pde_t;
typedef unsigned int pte_t;

/* Definition of Global Page Directory/Table */
extern pde_t page_directory[PAGE_DIRECTORY_SIZE];
extern pte_t page_table_0[PAGE_TABLE_SIZE];

/* Global Function */
extern void init_paging(void);

#endif
