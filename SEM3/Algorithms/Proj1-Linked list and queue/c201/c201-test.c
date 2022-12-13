#include "c201.h"
#include "c201-test-utils.h"
#include <stdio.h>

TEST(test_init, "Inicializace struktury")
	List_Init(test_list);

	test_print_list(test_list);
ENDTEST

TEST(test_get_first_empty, "Získání hodnoty prvního prvku v prázdném seznamu")
	List_Init(test_list);
	test_print_list(test_list);

	int test_result;
	List_GetFirst(test_list, &test_result);
ENDTEST

TEST(test_first_empty, "Aktivace prvního prvku v prázdném seznamu")
	List_Init(test_list);
	test_print_list(test_list);

	List_First(test_list);
	test_print_active_element(test_list);
ENDTEST

TEST(test_insert_first, "Vkládání nových prvků na první místo")
	List_Init(test_list);

	List_InsertFirst(test_list, 4);
	List_InsertFirst(test_list, 3);
	List_InsertFirst(test_list, 2);
	List_InsertFirst(test_list, 1);

	test_print_list(test_list);
ENDTEST

TEST(test_get_value_inactive, "Získání hodnoty prvku v seznamu bez aktivního prvku")
	List_Init(test_list);

	List_InsertFirst(test_list, 3);
	List_InsertFirst(test_list, 2);
	List_InsertFirst(test_list, 1);
	test_print_list(test_list);

	int test_result;
	List_GetValue(test_list, &test_result);
ENDTEST

TEST(test_set_value_inactive, "Aktualizace hodnoty prvku v seznamu bez aktivního prvku")
	List_Init(test_list);

	List_InsertFirst(test_list, 3);
	List_InsertFirst(test_list, 2);
	List_InsertFirst(test_list, 1);
	test_print_list(test_list);

	List_SetValue(test_list, 3);
	test_print_list(test_list);
ENDTEST

TEST(test_next_inactive, "Aktivace následujícího prvku v seznamu bez aktivního prvku")
	List_Init(test_list);

	List_InsertFirst(test_list, 3);
	List_InsertFirst(test_list, 2);
	List_InsertFirst(test_list, 1);
	test_print_list(test_list);

	List_Next(test_list);
	test_print_active_element(test_list);
ENDTEST

TEST(test_get_first, "Získání hodnoty prvního prvku")
	List_Init(test_list);

	List_InsertFirst(test_list, 3);
	List_InsertFirst(test_list, 2);
	List_InsertFirst(test_list, 1);
	test_print_list(test_list);

	int test_result;
	List_GetFirst(test_list, &test_result);
	printf("%-15s: %d\n", "Returned value", test_result);
ENDTEST

TEST(test_get_value, "Získání hodnot aktivních prvků")
	List_Init(test_list);

	List_InsertFirst(test_list, 3);
	List_InsertFirst(test_list, 2);
	List_InsertFirst(test_list, 1);
	test_print_list(test_list);

	int test_result;
	List_First(test_list);
	List_GetValue(test_list, &test_result);
	printf("%-15s: %d\n", "Returned value", test_result);

	List_Next(test_list);
	List_GetValue(test_list, &test_result);
	printf("%-15s: %d\n", "Returned value", test_result);

	List_Next(test_list);
	List_GetValue(test_list, &test_result);
	printf("%-15s: %d\n", "Returned value", test_result);
ENDTEST

TEST(test_set_value, "Aktualizace hodnot aktivních prvků")
	List_Init(test_list);

	List_InsertFirst(test_list, 3);
	List_InsertFirst(test_list, 2);
	List_InsertFirst(test_list, 1);

	List_SetValue(test_list, 99);
	test_print_list(test_list);

	List_First(test_list);
	List_SetValue(test_list, 3);
	test_print_list(test_list);

	List_Next(test_list);
	List_Next(test_list);
	List_SetValue(test_list, 1);

	test_print_list(test_list);
ENDTEST

TEST(test_first_next_activity, "Nastavení aktivity na následnících")
	List_Init(test_list);
	test_print_active_element(test_list);

	List_InsertFirst(test_list, 3);
	List_InsertFirst(test_list, 2);
	List_InsertFirst(test_list, 1);

	List_First(test_list);
	test_print_list(test_list);

	List_Next(test_list);
	test_print_active_element(test_list);

	List_Next(test_list);
	test_print_active_element(test_list);

	List_Next(test_list);
	test_print_active_element(test_list);
ENDTEST

TEST(test_is_active, "Aktivita seznamu")
	List_Init(test_list);

	List_InsertFirst(test_list, 3);
	List_InsertFirst(test_list, 2);
	List_InsertFirst(test_list, 1);
	test_print_list(test_list);
	printf("Is list element active? %s\n", List_IsActive(test_list) ? "Yes" : "No");

	List_First(test_list);
	printf("Is list element active? %s\n", List_IsActive(test_list) ? "Yes" : "No");

	List_Next(test_list);
	printf("Is list element active? %s\n", List_IsActive(test_list) ? "Yes" : "No");

	List_Next(test_list);
	printf("Is list element active? %s\n", List_IsActive(test_list) ? "Yes" : "No");

	List_Next(test_list);
	printf("Is list element active? %s\n", List_IsActive(test_list) ? "Yes" : "No");
ENDTEST

TEST(test_insert_after, "Vložení několika prvků na poslední místo")
	List_Init(test_list);

	List_InsertFirst(test_list, 1);
	test_print_list(test_list);

	List_First(test_list);
	List_InsertAfter(test_list, 2);
	test_print_list(test_list);

	List_Next(test_list);
	List_InsertAfter(test_list, 3);
	test_print_list(test_list);

	List_Next(test_list);
	List_InsertAfter(test_list, 4);
	test_print_list(test_list);
ENDTEST

TEST(test_delete_first, "Zrušení prvního prvku v seznamu")
	List_Init(test_list);

	List_InsertFirst(test_list, 1);
	List_InsertFirst(test_list, 2);
	List_InsertFirst(test_list, 3);
	test_print_list(test_list);

	List_DeleteFirst(test_list);
	test_print_list(test_list);

	List_DeleteFirst(test_list);
	test_print_list(test_list);

	List_DeleteFirst(test_list);
	test_print_list(test_list);

	List_DeleteFirst(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_delete_after, "Zrušení prvku po aktivním prvku")
	List_Init(test_list);

	List_InsertFirst(test_list, 3);
	List_InsertFirst(test_list, 3);
	List_InsertFirst(test_list, 2);
	List_InsertFirst(test_list, 2);
	List_InsertFirst(test_list, 1);
	List_InsertFirst(test_list, 1);

	List_DeleteAfter(test_list);
	test_print_list(test_list);

	List_First(test_list);
	List_DeleteAfter(test_list);
	test_print_list(test_list);

	List_Next(test_list);
	List_DeleteAfter(test_list);
	test_print_list(test_list);

	List_Next(test_list);
	List_DeleteAfter(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_dispose, "Zrušení prvků seznamu")
	List_Init(test_list);

	List_InsertFirst(test_list, 3);
	List_InsertFirst(test_list, 2);
	List_InsertFirst(test_list, 1);
	test_print_list(test_list);

	List_Dispose(test_list);
	test_print_list(test_list);
ENDTEST

int main()
{
	printf("C201 - Linked List, Basic Tests\n");
	printf("===============================\n");

	test_init();
	test_get_first_empty();
	test_first_empty();
	test_insert_first();
	test_get_value_inactive();
	test_set_value_inactive();
	test_next_inactive();
	test_get_first();
	test_get_value();
	test_set_value();
	test_is_active();
	test_first_next_activity();
	test_insert_after();
	test_delete_first();
	test_delete_after();
	test_dispose();

	return 0;
}

/* Konec c201-test.c */
