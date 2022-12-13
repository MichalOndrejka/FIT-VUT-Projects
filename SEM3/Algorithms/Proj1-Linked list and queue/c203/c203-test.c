#include "c203.h"
#include "c203-test-utils.h"
#include <stdio.h>

TEST(test_init, "Inicializace fronty")
	Queue_Init(test_queue);

	test_print_queue(test_queue);
ENDTEST

TEST(test_empty_empty, "Kontrola prázdnosti fronty v prázdné frontě")
	Queue_Init(test_queue);
	test_print_queue(test_queue);

	printf("Is queue empty? %s\n", Queue_IsEmpty(test_queue) ? "Yes" : "No");
ENDTEST

TEST(test_full_empty, "Kontrola plnosti fronty v prázdné frontě")
	Queue_Init(test_queue);
	test_print_queue(test_queue);

	printf("Is queue full? %s\n", Queue_IsFull(test_queue) ? "Yes" : "No");
ENDTEST

TEST(test_front_empty, "Výpis čela v prázdné frontě")
	Queue_Init(test_queue);
	test_print_queue(test_queue);

	char test_result;
	Queue_Front(test_queue, &test_result);
ENDTEST

TEST(test_remove_empty, "Odstranění čela v prázdné frontě")
	Queue_Init(test_queue);
	test_print_queue(test_queue);

	Queue_Remove(test_queue);
ENDTEST

TEST(test_get_empty, "Uvolnění čela v prázdné frontě")
	Queue_Init(test_queue);
	test_print_queue(test_queue);

	char test_result;
	Queue_Dequeue(test_queue, &test_result);
ENDTEST

TEST(test_up, "Vložení do fronty")
	Queue_Init(test_queue);
	test_print_queue(test_queue);

	Queue_Enqueue(test_queue, 'H');
	test_print_queue(test_queue);

	Queue_Enqueue(test_queue, 'e');
	test_print_queue(test_queue);

	Queue_Enqueue(test_queue, 'l');
	test_print_queue(test_queue);

	Queue_Enqueue(test_queue, 'l');
	test_print_queue(test_queue);

	Queue_Enqueue(test_queue, 'o');
	test_print_queue(test_queue);
ENDTEST

TEST(test_up_full, "Vložení do plné fronty")
	Queue_Init(test_queue);

	test_fill_queue(test_queue, QUEUE_SIZE - 1);
	test_print_queue(test_queue);

	Queue_Enqueue(test_queue, 'H');
	Queue_Enqueue(test_queue, 'i');
	Queue_Enqueue(test_queue, '!');

	test_print_queue(test_queue);
ENDTEST

TEST(test_empty_full, "Kontrola prázdnosti fronty v plné frontě")
	Queue_Init(test_queue);

	test_fill_queue(test_queue, QUEUE_SIZE - 1);
	test_print_queue(test_queue);

	printf("Is queue empty? %s\n", Queue_IsEmpty(test_queue) ? "Yes" : "No");
ENDTEST

TEST(test_full_full, "Kontrola plnosti fronty v plné frontě")
	Queue_Init(test_queue);

	test_fill_queue(test_queue, QUEUE_SIZE - 1);
	test_print_queue(test_queue);

	printf("Is queue full? %s\n", Queue_IsFull(test_queue) ? "Yes" : "No");
ENDTEST

TEST(test_remove, "Odstranění z čela fronty")
	Queue_Init(test_queue);
	test_fill_queue(test_queue, 3);
	test_print_queue(test_queue);

	Queue_Remove(test_queue);
	test_print_queue(test_queue);

	Queue_Remove(test_queue);
	test_print_queue(test_queue);

	Queue_Remove(test_queue);
	test_print_queue(test_queue);
ENDTEST

TEST(test_get, "Uvolnění z čela fronty")
	Queue_Init(test_queue);
	test_fill_queue(test_queue, 3);
	test_print_queue(test_queue);

	char test_result;
	Queue_Dequeue(test_queue, &test_result);
	test_print_queue(test_queue);
	printf("%-15s: %c\n", "Returned value", test_result);

	Queue_Dequeue(test_queue, &test_result);
	test_print_queue(test_queue);
	printf("%-15s: %c\n", "Returned value", test_result);

	Queue_Dequeue(test_queue, &test_result);
	test_print_queue(test_queue);
	printf("%-15s: %c\n", "Returned value", test_result);
ENDTEST

int main()
{
	QUEUE_SIZE = 10;

	printf("C203 - Linked List, Basic Tests\n");
	printf("===============================\n");

	test_init();
	test_empty_empty();
	test_full_empty();
	test_front_empty();
	test_remove_empty();
	test_get_empty();
	test_up();
	test_up_full();
	test_empty_full();
	test_full_full();
	test_remove();
	test_get();

	return 0;
}

/* Konec c203-test.c */
