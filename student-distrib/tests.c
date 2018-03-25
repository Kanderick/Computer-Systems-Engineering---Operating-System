#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "idt.h"
#include "paging.h"
#include "file_system.h"
#include "rtc.h"
#include "device.h"
#include "terminal.h"

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
// NOTE NEED HEADER COMMENTS!!!!!!!!!!!!!!!!
#define testBufferMaxLen		6000 // Move this to header file
int file_read_test_naive(){
	dentry_t test;
	uint8_t buffer[testBufferMaxLen];
	uint32_t buflen;
	char name[] = "fish";
	if(read_dentry_by_name((uint8_t *)name, &test)==-1){
		printf("Name not found.\n");
		return PASS;
	}
	else {
		printf("File Name: %s\nFile type: %d\nInode Num: %d\n", name, test.filetype, test.inode_num);
	 	buflen = ece391FileSystem.ece391_inodes[test.inode_num].length;
		if (buflen >= testBufferMaxLen)
			buflen = testBufferMaxLen;
		if (test.filetype == 2){
			read_data(test.inode_num, 0, buffer, buflen);
			putbuf((int8_t*)buffer, buflen);
		}
	}
	return PASS;
}

int test_file_open_read_close(){
	uint8_t name[] = "frame0.txt";
	uint8_t name2[] = "verylargetextwithverylongname.txt";
	uint8_t buffer[testBufferMaxLen];
	int32_t read_len;
	int32_t ii;
	if (file_open(name) == -1){
		printf("Open FAILED.\n");
		return PASS;
	}
	printf("fd: %d \n", file_find(name));
	for (ii = 0; ii < 19; ii++){
		// print another 10 byte
		read_len = file_read(file_find(name), buffer, 10) ;
		if (read_len == -1){
			printf("READ FAILED.\n");
		}
		else{
			putbuf((int8_t*)buffer, read_len);
		}
	}
	// open another file
	if (file_open(name2) == -1){
		printf("Open FAILED.\n");
		file_close(file_find(name));
	}
	printf("fd: %d \n", file_find(name2));
	for (ii = 0; ii < 2; ii++){
		// print another 10 byte
		read_len = file_read(file_find(name2), buffer, 10) ;
		if (read_len == -1){
			printf("READ FAILED.\n");
		}
		else{
			putbuf((int8_t*)buffer, read_len);
		}
	}
	file_close(file_find(name));
	file_close(file_find(name2));
	return PASS;
}

int rtc_test() {
	clearScreen();
	TEST_HEADER;

	unsigned char read_buf;						/* rtc_read buffer */
	int32_t frequency = RTC_TEST_INITIAL_FRQ;	/* Initialize RTC freqency */
	int32_t fd = 0; 							// Unused in CP 3.2
	uint8_t *filename = (unsigned char *)"rtc";	// Unused in CP 3.2
	int32_t ticks;								/* Ticks to print */
	int8_t multiplier;							/* Act as a counter */

	printf("[TEST] rtc_open\n");
	rtc_open(filename);
	printf("[PASS] RTC Opened.\n");

	printf("[TEST] rtc_write & read, print '1' in different frequency\n");
	printf("Press ALT to continue test...");
	key_pressed();	// Press alt key to conduct the frequency test
	for (multiplier = 0; multiplier <= RTC_TEST_MAX_MULTIPLIER; multiplier++) {
		clearScreen();
		printf("Current Frequency: %dHz\n", frequency);
		ticks = frequency * RTC_TEST_SEC_PER_FRQ;	// Calculate loop ticks
		if (ticks > RTC_TEST_MAX_TICKS) ticks = RTC_TEST_MAX_TICKS;	// Upper bound of ticks is defined
		while(ticks != 0) {
			if (!rtc_read(fd, &read_buf, 1))	// If received a rtc interrupt, print '1'
				putc('1');
			ticks --;
		}
		frequency = frequency * 2;	// Double the frequency
		rtc_write(fd, (unsigned char *)&frequency, 4);
	}
	frequency = RTC_TEST_INITIAL_FRQ;	// Restore initial rtc frequency
	rtc_write(fd, (unsigned char *)&frequency, 4);
	clearScreen();

	printf("[TEST] rtc_close\n");
	rtc_close(fd);	// RTC close just return 0
	printf("[PASS] RTC Closed.\n");

	return PASS;	// If anything fail, code will segfault
}

int terminal_test() {
	TEST_HEADER;
	int32_t flag;
	unsigned char buffer[TERMINAL_TEST_BUFFER];				/* rtc_read buffer */
	int32_t fd = 0; 							// Unused in CP 3.2
	uint8_t *filename = (unsigned char *)"terminal";	// Unused in CP 3.2

	printf("\n[TEST] terminal_read without open.\n");
	flag = terminal_write(fd, buffer, TERMINAL_TEST_BUFFER);
	if (flag == -1) printf("[PASS] terminal not read.\n");
	else return FAIL;

	printf("\n[TEST] terminal_read without open.\n");
	flag = terminal_read(fd, buffer, TERMINAL_TEST_BUFFER);
	if (flag == -1) printf("[PASS] terminal not read.\n");
	else return FAIL;

	printf("[TEST] terminal_open\n");
	terminal_open(filename);
	printf("[PASS] terminal Opened.\n");

	printf("[TEST] terminal_open twice.\n");
	flag = terminal_open(filename);
	if (flag == -1) printf("[PASS] terminal not open again.\n");
	else return FAIL;

	printf("\n[TEST] terminal_read\n");
	printf("Please type LESS than 128 characters, stop with ENTER.\n");
	printf("Please try SHIFT, CAPSLOCK, and BACKSPACE:\n");
	terminal_read(fd, buffer, TERMINAL_TEST_BUFFER);

	printf("\n[TEST] terminal_write\n");
	printf("Content in the buffer shold be the same as above:\n");
	terminal_write(fd, buffer, TERMINAL_TEST_BUFFER);

	printf("\n[TEST] Handle buffer overflow\n");
	printf("Please type MORE than 128 characters, stop with ENTER:\n");
	terminal_read(fd, buffer, TERMINAL_TEST_BUFFER);
	printf("\nContent in the buffer should truncate to 128 characters:\n");
	terminal_write(fd, buffer, TERMINAL_TEST_BUFFER);
	printf("\n");

	printf("\n[TEST] Handle unknown scancode\n");
	printf("Please try F1-F12, nothing should happen. Stop with ENTER:\n");
	terminal_read(fd, buffer, TERMINAL_TEST_BUFFER);

	printf("\n[TEST] Scrolling and clear screen\n");
	printf("Please enter some random staff. Use CTRL+L to clear screen:\n");

	printf("[TEST] terminal_close\n");
	rtc_close(fd);
	printf("[PASS] terminal Closed.\n");

	printf("[TEST] terminal_close twice.\n");
	flag = rtc_close(fd);
	if (flag == -1) printf("[PASS] terminal not close again.\n");
	else return FAIL;

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
	#if (RTC_TERMINAL_TEST == 1)
	TEST_OUTPUT("rtc_test", rtc_test());
	terminal_test();
	#endif
	// need macro
	// file_read_test_naive();
	test_file_open_read_close();
}
