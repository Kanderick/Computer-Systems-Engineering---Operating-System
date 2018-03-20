#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "idt.h"
#include "paging.h"
#include "file_system.h"

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}

/* Checkpoint 1 tests */
/* idt_test
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;
	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) &&
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}
	return result;
}

/* paging_test
 * Purpose	Check page directory and page table alignment, content, and dereference
 * Inputs	None
 * Outputs	PASS/FAIL
 * Side Effects
 *		Kernel will freeze after PFE
 * Coverage
 *		Page directory and page table alignment, content, and dereference
 * Files	paging.c/h
 */
int paging_test() {
	clear();
	printf("\n\n\n");
	TEST_HEADER;

	int result = PASS;
	unsigned long test_variable;

	/* Page Directory and Page Table should align at 4kb */
	printf("\n[TEST page_directory and page_table_0 address]\n");
	if (((unsigned long)page_directory & PAGE_TEST_ADDR_MASK) != 0) {
		printf("[FAIL] page_directory     0x%#x, not aligned at 4KB\n", page_directory);
		result = FAIL;
	} else {
		printf("[PASS] *page_directory    0x%#x, aligned at 4KB\n", page_directory);
	}
	if (((unsigned long)page_table_0 & PAGE_TEST_ADDR_MASK) != 0) {
		printf("[FAIL] page_table_0       0x%#x, not aligned at 4KB\n", page_table_0);
		result = FAIL;
	} else {
		printf("[PASS] *page_table_0      0x%#x, aligned at 4KB\n", page_table_0);
	}

	/* Check if Page Directory and Page Table contain correct content */
	printf("\n[TEST page_directory and page_table_0 content]\n");
	if ((page_directory[0] & PTBA_MASK) != (unsigned long)page_table_0) {
		printf("[FAIL] page_directory[0]  0x%#x, doesn't contain address of page_table_0\n", page_directory[0]);
		result = FAIL;
	} else {
		printf("[PASS] page_directory[0]  0x%#x, contain addr of page_table_0 with flags\n", page_directory[0]);
	}
	if ((page_directory[1] & PBA_4M_MASK) != KERNEL_START) {
		printf("[FAIL] page_directory[1]  0x%#x, doesn't contain address of kernel\n", page_directory[1]);
		result = FAIL;
	} else {
		printf("[PASS] page_directory[1]  0x%#x, contain addr of kernel with flags\n", page_directory[1]);
	}
	if (page_directory[2] != 0) {
		printf("[FAIL] page_directory[2]  0x%#x, not empty\n", page_directory[2]);
		result = FAIL;
	} else {
		printf("[PASS] page_directory[2]  0x%#x, empty\n", page_directory[2]);
	}
	if (page_table_0[0] != 0) {
		printf("[FAIL] page_table_0[0]    0x%#x, not empty\n", page_table_0[0]);
		result = FAIL;
	} else {
		printf("[PASS] page_table_0[0]    0x%#x, empty\n", page_table_0[0]);
	}
	if ((page_table_0[VIDEO_VIRTUAL] & PBA_4K_MASK) != VIDEO_START) {
		printf("[FAIL] page_table_0[0x%x] 0x%#x, doesn't contain address of video mem\n", VIDEO_VIRTUAL, page_table_0[VIDEO_VIRTUAL]);
		result = FAIL;
	} else {
		printf("[PASS] page_table_0[0x%x] 0x%#x, contain addr of video mem with flags\n", VIDEO_VIRTUAL, page_table_0[VIDEO_VIRTUAL]);
	}

	/* If anything trigger PFE, test failed */
	printf("\n[TEST dereference at kernel address]\n");
	test_variable = *((unsigned long *)KERNEL_START);
	printf("[PASS] M[%#x] is     0x%#x, didn't trigger PFE\n", KERNEL_START, test_variable);
	test_variable = *((unsigned long *)PAGE_TEST_KERNEL_ADDR);
	printf("[PASS] M[%#x] is     0x%#x, didn't trigger PFE\n", PAGE_TEST_KERNEL_ADDR, test_variable);

	printf("\n[TEST dereference at video address]\n");
	test_variable = *((unsigned long *)VIDEO_START);
	printf("[PASS] M[%#x] is     0x%#x, didn't trigger PFE\n", VIDEO_START, test_variable);
	test_variable = *((unsigned long *)PAGE_TEST_VIDEO_ADDR);
	printf("[PASS] M[%#x] is     0x%#x, didn't trigger PFE\n", PAGE_TEST_VIDEO_ADDR, test_variable);

	/* If PFE didn't triggered, test failed */
	printf("\n[TEST dereference at unpresent address, should trigger PFE]\n");
	test_variable = *((unsigned long *)PAGE_TEST_UNPRESENT);
	printf("[FAIL] M[%#x] is     0x%#x, didn't trigger PFE\n", PAGE_TEST_UNPRESENT, test_variable);
	result = FAIL;	/* Test failed if not triggered exception */

	return result;	/* Reserved for future use */
}

#if (EXCEPTION_TEST == 1)
/* divide_zero_test
 * Purpose	Check if Divide Zero Exception is working
 * Inputs	None
 * Outputs	PASS/FAIL
 * Side Effects
 *		Kernel will freeze after exception
 * Coverage
 *		Divide Zero Exception
 * Files	idt.c/h
 */
int divide_zero_test(){
	TEST_HEADER;
	/* Compiler will not be happy with this line of code */
	int i = 5 / 0;	/* This will trigger divide zero exception. */
	return FAIL;	/* Test failed if not triggered exception */
}
#endif

/* invalid_opcode_test
 * Purpose	Check if Invalid Opcode Exception is working
 * Inputs	None
 * Outputs	PASS/FAIL
 * Side Effects
 *		Kernel will freeze after exception
 * Coverage
 *		Invalid Opcode Exception
 * Files	idt.c/h
 */
int invalid_opcode_test(){
	TEST_HEADER;
	int val = 1;
	asm volatile("movl %0,%%cr6": :"r" (val)); /* CR6 is a madeup register */
	return FAIL;	/* Test failed if not triggered exception */
}

/* Checkpoint 2 tests */
int file_read_test_naive(){
	dentry_t test;
	uint8_t buffer[6000];
	char name[] = "very";
	if(read_dentry_by_name((uint8_t *)name, &test)==-1){
		printf("Name not found.\n");
		return PASS;
	}
	else {
		printf("File Name: %s\nFile type: %d\nInode Num: %d\n", name, test.filetype, test.inode_num);
		if (test.filetype == 2){
			read_data(test.inode_num, 0, buffer, 6000);
			buffer[5999]=NULL;
			printf("%s", buffer);
		}
	}
	return PASS;
}

/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */

/* Test suite entry point */
void launch_tests(){
	#if (IDT_TEST == 1)
	TEST_OUTPUT("idt_test", idt_test());
	#endif
	#if (PAGE_TEST == 1)
	TEST_OUTPUT("paging_test", paging_test());
	#endif
	#if (EXCEPTION_TEST == 1)
	TEST_OUTPUT("divide_zero_test", divide_zero_test());
	#endif
	#if (EXCEPTION_TEST == 2)
	TEST_OUTPUT("invalid_opcode_test", invalid_opcode_test());
	#endif
	file_read_test_naive();
}
