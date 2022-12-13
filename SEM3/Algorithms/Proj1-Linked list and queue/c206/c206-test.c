/*
 *  Předmět: Algoritmy (IAL) - FIT VUT v Brně
 *  Základní testy pro příklad c206.c (Dvousměrně vázaný lineární seznam)
 *  Vytvořil: Martin Tuček, září 2005
 *  Úprava: Kamil Jeřábek, září 2019
 *          Daniel Dolejška, září 2021
 */

#include "c206-test-utils.h"
#include <stdio.h>

TEST(test_init, "Inicializace seznamu")
	DLL_Init(test_list);

	test_print_list(test_list);
ENDTEST

TEST(test_get_first_empty, "Získání hodnoty prvního prvku v prázdném seznamu")
	DLL_Init(test_list);
	test_print_list(test_list);

	int test_result;
	DLL_GetFirst(test_list, &test_result);
ENDTEST

TEST(test_get_last_empty, "Získání hodnoty posledního prvku v prázdném seznamu")
	DLL_Init(test_list);
	test_print_list(test_list);

	int test_result;
	DLL_GetFirst(test_list, &test_result);
ENDTEST

TEST(test_first_empty, "Aktivace prvního prvku v prázdném seznamu")
	DLL_Init(test_list);
	test_print_list(test_list);

	DLL_First(test_list);
	test_print_active_element(test_list);
ENDTEST

TEST(test_last_empty, "Aktivace posledního prvku v prázdném seznamu")
	DLL_Init(test_list);
	test_print_list(test_list);

	DLL_Last(test_list);
	test_print_active_element(test_list);
ENDTEST

TEST(test_insert_first, "Vkládání nových prvků na začátek seznamu")
	DLL_Init(test_list);
	test_print_list(test_list);

	DLL_InsertFirst(test_list, 4);
	test_print_list(test_list);

	DLL_InsertFirst(test_list, 3);
	test_print_list(test_list);

	DLL_InsertFirst(test_list, 2);
	test_print_list(test_list);

	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);
ENDTEST

TEST(test_insert_last, "Vkládání nových prvků na konec seznamu")
	DLL_Init(test_list);
	test_print_list(test_list);

	DLL_InsertLast(test_list, 1);
	test_print_list(test_list);

	DLL_InsertLast(test_list, 2);
	test_print_list(test_list);

	DLL_InsertLast(test_list, 3);
	test_print_list(test_list);

	DLL_InsertLast(test_list, 4);
	test_print_list(test_list);
ENDTEST

TEST(test_get_value_inactive, "Získání hodnoty prvku v seznamu bez aktivního prvku")
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	int test_result;
	DLL_GetValue(test_list, &test_result);
ENDTEST

TEST(test_set_value_inactive, "Aktualizace hodnoty prvku v seznamu bez aktivního prvku")
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	DLL_SetValue(test_list, 3);
	test_print_list(test_list);
ENDTEST

TEST(test_next_inactive, "Aktivace následujícího prvku v seznamu bez aktivního prvku")
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	DLL_Next(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_previous_inactive, "Aktivace předcházejícího prvku v seznamu bez aktivního prvku")
	DLL_Init(test_list);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	DLL_Previous(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_get_first, "Získání hodnoty prvního prvku")
	DLL_Init(test_list);

	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	int test_result;
	DLL_GetFirst(test_list, &test_result);
	printf("%-15s: %d\n", "Returned value", test_result);
ENDTEST

TEST(test_get_last, "Získání hodnoty posledního prvku")
	DLL_Init(test_list);

	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	int test_result;
	DLL_GetLast(test_list, &test_result);
	printf("%-15s: %d\n", "Returned value", test_result);
ENDTEST

TEST(test_get_value, "Získání hodnot aktivních prvků")
	DLL_Init(test_list);

	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	int test_result;
	DLL_First(test_list);
	DLL_GetValue(test_list, &test_result);
	printf("%-15s: %d\n", "Returned value", test_result);

	DLL_Next(test_list);
	DLL_GetValue(test_list, &test_result);
	printf("%-15s: %d\n", "Returned value", test_result);

	DLL_Next(test_list);
	DLL_GetValue(test_list, &test_result);
	printf("%-15s: %d\n", "Returned value", test_result);
ENDTEST

TEST(test_set_value, "Aktualizace hodnot aktivních prvků")
	DLL_Init(test_list);

	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);

	DLL_SetValue(test_list, 99);
	test_print_list(test_list);

	DLL_First(test_list);
	DLL_SetValue(test_list, 3);
	test_print_list(test_list);

	DLL_Next(test_list);
	DLL_Next(test_list);
	DLL_SetValue(test_list, 1);

	test_print_list(test_list);
ENDTEST

TEST(test_first_next_activity, "Nastavení aktivity na následnících")
	DLL_Init(test_list);
	test_print_active_element(test_list);

	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);

	DLL_First(test_list);
	test_print_list(test_list);

	DLL_Next(test_list);
	test_print_active_element(test_list);

	DLL_Next(test_list);
	test_print_active_element(test_list);

	DLL_Next(test_list);
	test_print_active_element(test_list);
ENDTEST

TEST(test_last_previous_activity, "Nastavení aktivity na předchůdcích")
	DLL_Init(test_list);
	test_print_active_element(test_list);

	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);

	DLL_Last(test_list);
	test_print_list(test_list);

	DLL_Previous(test_list);
	test_print_active_element(test_list);

	DLL_Previous(test_list);
	test_print_active_element(test_list);

	DLL_Previous(test_list);
	test_print_active_element(test_list);
ENDTEST

TEST(test_is_active, "Aktivita seznamu")
	DLL_Init(test_list);

	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);
	printf("Is list element active? %s\n", DLL_IsActive(test_list) ? "Yes" : "No");

	DLL_First(test_list);
	printf("Is list element active? %s\n", DLL_IsActive(test_list) ? "Yes" : "No");

	DLL_Next(test_list);
	printf("Is list element active? %s\n", DLL_IsActive(test_list) ? "Yes" : "No");

	DLL_Previous(test_list);
	printf("Is list element active? %s\n", DLL_IsActive(test_list) ? "Yes" : "No");

	DLL_Next(test_list);
	printf("Is list element active? %s\n", DLL_IsActive(test_list) ? "Yes" : "No");

	DLL_Next(test_list);
	printf("Is list element active? %s\n", DLL_IsActive(test_list) ? "Yes" : "No");
ENDTEST

TEST(test_insert_after, "Vložení několika prvků po aktivním prvku")
	DLL_Init(test_list);

	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	DLL_First(test_list);
	DLL_InsertAfter(test_list, 2);
	test_print_list(test_list);

	DLL_Last(test_list);
	DLL_InsertAfter(test_list, 3);
	test_print_list(test_list);

	DLL_Next(test_list);
	DLL_InsertAfter(test_list, 4);
	test_print_list(test_list);
ENDTEST

TEST(test_insert_before, "Vložení několika prvků po před aktivní prvek")
	DLL_Init(test_list);

	DLL_InsertFirst(test_list, 4);
	test_print_list(test_list);

	DLL_Last(test_list);
	DLL_InsertBefore(test_list, 3);
	test_print_list(test_list);

	DLL_First(test_list);
	DLL_InsertBefore(test_list, 2);
	test_print_list(test_list);

	DLL_Previous(test_list);
	DLL_InsertBefore(test_list, 1);
	test_print_list(test_list);
ENDTEST

TEST(test_delete_first, "Zrušení prvního prvku v seznamu")
	DLL_Init(test_list);

	DLL_InsertFirst(test_list, 1);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 3);
	test_print_list(test_list);

	DLL_DeleteFirst(test_list);
	test_print_list(test_list);

	DLL_DeleteFirst(test_list);
	test_print_list(test_list);

	DLL_DeleteFirst(test_list);
	test_print_list(test_list);

	DLL_DeleteFirst(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_delete_last, "Zrušení posledního prvku v seznamu")
	DLL_Init(test_list);

	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	DLL_DeleteLast(test_list);
	test_print_list(test_list);

	DLL_DeleteLast(test_list);
	test_print_list(test_list);

	DLL_DeleteLast(test_list);
	test_print_list(test_list);

	DLL_DeleteLast(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_delete_before, "Zrušení prvku před aktivním prvkem")
	DLL_Init(test_list);

	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	DLL_Last(test_list);
	DLL_DeleteBefore(test_list);
	test_print_list(test_list);

	DLL_Previous(test_list);
	DLL_DeleteBefore(test_list);
	test_print_list(test_list);

	DLL_Previous(test_list);
	DLL_DeleteBefore(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_delete_after, "Zrušení prvku po aktivním prvku")
	DLL_Init(test_list);

	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	DLL_First(test_list);
	DLL_DeleteAfter(test_list);
	test_print_list(test_list);

	DLL_Next(test_list);
	DLL_DeleteAfter(test_list);
	test_print_list(test_list);

	DLL_Next(test_list);
	DLL_DeleteAfter(test_list);
	test_print_list(test_list);
ENDTEST

TEST(test_dispose, "Zrušení prvků seznamu")
	DLL_Init(test_list);

	DLL_InsertFirst(test_list, 3);
	DLL_InsertFirst(test_list, 2);
	DLL_InsertFirst(test_list, 1);
	test_print_list(test_list);

	DLL_Dispose(test_list);
	test_print_list(test_list);
ENDTEST

int main()
{
	printf("C206 - Doubly Linked List, Basic Tests\n");
	printf("======================================\n");

	test_init();
	test_get_first_empty();
	test_get_last_empty();
	test_first_empty();
	test_last_empty();
	test_insert_first();
	test_insert_last();
	test_get_value_inactive();
	test_set_value_inactive();
	test_next_inactive();
	test_previous_inactive();
	test_get_first();
	test_get_last();
	test_get_value();
	test_set_value();
	test_first_next_activity();
	test_last_previous_activity();
	test_is_active();
	test_insert_after();
	test_insert_before();
	test_delete_first();
	test_delete_last();
	test_delete_before();
	test_delete_after();
	test_dispose();

	return 0;
}

/* Konec c206-test.c */
