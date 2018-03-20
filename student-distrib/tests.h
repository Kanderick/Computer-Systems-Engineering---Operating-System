#ifndef TESTS_H
#define TESTS_H

/* General Test Macro */
#define PASS 1
#define FAIL 0

/* General Test Function */
void launch_tests();

/* Checkpoint 1 Tests Macro and Functions */
/* Test keyboard with everything set to 0 */
#define RTC_TEST                0           /* Set to 1 to run RTC test */
#define IDT_TEST                0           /* Set to 1 to run IDT test */
#define PAGE_TEST               0           /* Set to 1 to run paging test */
#define EXCEPTION_TEST          0           /* Set to 1 to get divide zero */
                                            /* Set to 2 to get invalid opcode */
#define PAGE_TEST_ADDR_MASK     0x00000FFF  /* Mask out other flags */
#define PAGE_TEST_UNPRESENT     0x00012000  /* Random page address */
#define PAGE_TEST_KERNEL_ADDR   0x0040000F  /* Random address within kernel page */
#define PAGE_TEST_VIDEO_ADDR    0x000B800F  /* Random address within video page */

int idt_test();
int paging_test();
#if (EXCEPTION_TEST == 1)
int divide_zero_test();
#endif
int invalid_opcode_test();

/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */

#endif /* TESTS_H */
