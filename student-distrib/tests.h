#ifndef TESTS_H
#define TESTS_H

#define PAGE_TEST 1

// test launcher
void launch_tests();
int idt_test();
#if (PAGE_TEST == 1)
int paging_test();
#endif

#endif /* TESTS_H */
