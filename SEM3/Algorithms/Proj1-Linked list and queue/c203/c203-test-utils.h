#ifndef C203_TEST_H
#define C203_TEST_H

#include "c203.h"
#include <stdio.h>

#define TEST(NAME, DESCRIPTION)                                                \
void NAME() {                                                                  \
	printf("\n[%s] %s\n", #NAME, DESCRIPTION);                                 \
    Queue _queue;                                                              \
	Queue *test_queue = &_queue;

#define ENDTEST                                                                \
	printf("\n");                                                              \
}

#define FAIL(REASON) printf("[FAILED] %s", REASON)

void test_print_queue(Queue *);
void test_fill_queue(Queue *, int);
void test_fill_queue_str(Queue *, char *);

#endif
