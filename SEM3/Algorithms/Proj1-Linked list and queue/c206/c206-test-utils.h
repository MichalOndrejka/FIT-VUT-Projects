#ifndef C206_TEST_H
#define C206_TEST_H

#include "c206.h"
#include <stdio.h>

#define TEST(NAME, DESCRIPTION)                                                \
void NAME() {                                                                  \
	printf("\n[%s] %s\n", #NAME, DESCRIPTION);                                 \
    DLList _list;                                                              \
	DLList *test_list = &_list;

#define ENDTEST                                                                \
	printf("\n");                                                              \
}

#define FAIL(REASON) printf("[FAILED] %s", REASON)

void test_print_list(DLList *);
void test_dispose_list(DLList *);

void test_print_first_element(DLList *list);
void test_print_last_element(DLList *list);
void test_print_active_element(DLList *list);

#endif
