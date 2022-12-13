#include "c203-test-utils.h"
#include <stdio.h>
#include <string.h>

void test_print_queue(Queue *queue)
{
	if (queue == NULL)
	{
		printf("The list is not initialized!\n");
		return;
	}

	printf("%-15s: ", "Queue elements");

	// Prints a queue content.
	for (int i = 0; i < QUEUE_SIZE; i++)
	{
		putchar(queue->array[i]);
	}
	putchar('\n');
	for (int i = 0; i < 17; putchar(' '), i++);

	// Prints a queue index(es).
	for (int i = 0; i < QUEUE_SIZE; i++)
	{
		if ((i == queue->firstIndex) || (i == queue->freeIndex))
		{
			putchar('^');
		}
		else
		{
			putchar(' ');
		}
	}
	putchar('\n');
	for (int i = 0; i < 17; putchar(' '), i++);

	// Prints index(es) meaning.
	for (int i = 0; i < QUEUE_SIZE; i++)
	{
		if ((i == queue->firstIndex) && (i == queue->freeIndex))
		{
			putchar('E');
		}
		else if (i == queue->firstIndex)
		{
			putchar('F');
		}
		else if (i == queue->freeIndex)
		{
			putchar('B');
		}
		else
		{
			putchar(' ');
		}
	}
	putchar('\n');
}

void test_fill_queue(Queue *queue, int count)
{
	for (int i = 0; i < count; i++)
	{
		Queue_Enqueue(queue, '0' + i);
	}
}

void test_fill_queue_str(Queue *queue, char *string)
{
	for (char *c = string; *c != 0; c++)
	{
		Queue_Enqueue(queue, *c);
	}
}
