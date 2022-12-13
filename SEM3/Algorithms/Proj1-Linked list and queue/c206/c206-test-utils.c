#include "c206-test-utils.h"
#include <stdio.h>
#include <string.h>

const int MaxListLength = 10;

void test_print_list(DLList *list)
{
	if (list == NULL)
	{
		printf("The list is not initialized!\n");
		return;
	}

	printf("%-15s:", "List elements");
	DLLElementPtr currentElement = list->firstElement;
	if (currentElement == NULL)
	{
		printf(" none (list is empty)");
	}

	int itemCount = 0;
	while (currentElement != NULL)
	{
		printf(" %d", currentElement->data);
		currentElement = currentElement->nextElement;
		if (++itemCount > MaxListLength)
		{
			printf("*ERROR* Wrong element linking detected!\n");
			break;
		}
	}

	printf("\n");
	test_print_first_element(list);
	test_print_last_element(list);
	test_print_active_element(list);
}

void test_dispose_list(DLList *list)
{
	if (list == NULL)
	{
		printf("List was not initialised yet!\n");
		return;
	}

	DLLElementPtr temporaryElement;
	while (list->firstElement != NULL)
	{
		temporaryElement = list->firstElement->nextElement;
		free(list->firstElement);
		list->firstElement = temporaryElement;
	}
}

void test_print_first_element(DLList *list)
{
	if (list->firstElement == NULL)
	{
		printf("%-15s: %s\n", "First element", "not initialised (list is empty)");
	}
	else
	{
		printf("%-15s: %d\n", "First element", list->firstElement->data);
	}
}

void test_print_last_element(DLList *list)
{
	if (list->lastElement == NULL)
	{
		printf("%-15s: %s\n", "Last element", "not initialised (list is empty)");
	}
	else
	{
		printf("%-15s: %d\n", "Last element", list->lastElement->data);
	}
}

void test_print_active_element(DLList *list)
{
	if (list->activeElement == NULL)
	{
		printf("%-15s: %s\n", "Active element", "not initialised (no item is active)");
	}
	else
	{
		printf("%-15s: %d\n", "Active element", list->activeElement->data);
	}
}
