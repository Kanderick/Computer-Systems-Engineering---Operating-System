#ifndef PAGE_H
#define PAGE_H

#include "lib.h"
#include "types.h"

#define PAGE_DIRECTORY_SIZE     1024
#define PAGE_TABLE_SIZE         1024
#define _4KB                    4096
#define _4MB                    4194304

#define PRESENT_MASK            0x00000001
#define R_W_MASK                0x00000002
#define U_S_MASK                0x00000004
#define PWT_MASK                0x00000008
#define PCD_MASK                0x00000010
#define ACCESSED_MASK           0x00000020
#define DIRTY_MASK              0x00000040
#define PAGE_SIZE_MASK          0x00000080
#define AVAILABLE_MASK          0x00000E00
#define KERNEL_START            0x00400000
#define VIDEO_START             0x000B8000
#define PTBA_MASK               0xFFFFF000
#define PBA_4K_MASK             0xFFFFF000
#define PBA_4M_MASK             0xFFC00000

#define CR0_PG_FLAG             0x80000000
#define CR4_PSE_FLAG            0x00000010
#define CR4_PAE_FLAG            0x00000020
#define CR4_PGE_FLAG            0x00000080

typedef unsigned int pde_t;
typedef unsigned int pte_t;

extern pde_t page_directory[PAGE_DIRECTORY_SIZE];
extern pte_t page_table_0[PAGE_TABLE_SIZE];

void init_paging(void);

#endif
