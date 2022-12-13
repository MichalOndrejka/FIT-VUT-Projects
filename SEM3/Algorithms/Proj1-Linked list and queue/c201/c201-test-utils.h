#ifndef C201_TEST_H
#define C201_TEST_H

#include "c201.h"
#include <stdio.h>

#define TEST(NAME, DESCRIPTION)                                                \
void NAME() {                                                                  \
	printf("\n[%s] %s\n", #NAME, DESCRIPTION);                                 \
    List _list;                                                                \
	List *test_list = &_list;

#define ENDTEST                                                                \
	printf("\n");                                                              \
	test_dispose_list(test_list);                                              \
}

#define FAIL(REASON) printf("[FAILED] %s", REASON)

void test_print_list(List *);
void test_dispose_list(List *);

void test_print_first_element(List *list);
void test_print_active_element(List *list);

#endif
