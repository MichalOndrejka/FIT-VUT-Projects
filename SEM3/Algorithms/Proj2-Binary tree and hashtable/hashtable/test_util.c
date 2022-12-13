#include "test_util.h"
#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>

ht_item_t *uninitialized_item;

void ht_print_item_value(float *value) {
  if (value != NULL) {
    printf("%.2f\n", *value);
  } else {
    printf("NULL\n");
  }
}

void ht_print_item(ht_item_t *item) {
  if (item != NULL) {
    printf("(%s,%.2f)\n", item->key, item->value);
  } else {
    printf("NULL\n");
  }
}

void ht_print_table(ht_table_t *table) {
  int max_count = 0;
  int sum_count = 0;

  printf("------------HASH TABLE--------------\n");
  for (int i = 0; i < HT_SIZE; i++) {
    printf("%i: ", i);
    int count = 0;
    ht_item_t *item = (*table)[i];
    while (item != NULL) {
      printf("(%s,%.2f)", item->key, item->value);
      if (item != uninitialized_item) {
        count++;
      }
      item = item->next;
    }
    printf("\n");
    if (count > max_count) {
      max_count = count;
    }
    sum_count += count;
  }

  printf("------------------------------------\n");
  printf("Total items in hash table: %i\n", sum_count);
  printf("Maximum hash collisions: %i\n", max_count == 0 ? 0 : max_count - 1);
  printf("------------------------------------\n");
}

void init_uninitialized_item() {
  uninitialized_item = (ht_item_t *)malloc(sizeof(ht_item_t));
  uninitialized_item->key = "*UNINITIALIZED*";
  uninitialized_item->value = -1;
  uninitialized_item->next = NULL;
}

void init_test_table(ht_table_t **table) {
  (*table) = (ht_table_t *)malloc(sizeof(ht_table_t));
  for (int i = 0; i < MAX_HT_SIZE; i++) {
    (**table)[i] = uninitialized_item;
  };
}

void ht_insert_many(ht_table_t *table, const ht_item_t items[], int count) {
  for (int i = 0; i < count; i++) {
    ht_insert(table, items[i].key, items[i].value);
  }
}
