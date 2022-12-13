/*
 * Hlavičkový súbor pre tabuľku s rozptýlenými položkami.
 * Tento súbor neupravujte.
 */

#ifndef IAL_HASHTABLE_H
#define IAL_HASHTABLE_H

#include <stdbool.h>

/*
 * Maximálna veľkosť poľa pre implementáciu tabuľky.
 * Funkcie pracujúce s tabuľkou uvažujú veľkosť HT_SIZE.
 */
#define MAX_HT_SIZE 101

/*
 * Veľkosť tabuľky s ktorou pracujú implementované funkcie.
 * Pre účely testovania je vhodné mať možnosť meniť veľkosť tabuľky.
 * Pre správne fungovanie musí byť veľkosť prvočíslom.
 */
extern int HT_SIZE;

// Prvok tabuľky
typedef struct ht_item {
  char *key;            // kľúč prvku
  float value;          // hodnota prvku
  struct ht_item *next; // ukazateľ na ďalšie synonymum
} ht_item_t;

// Tabuľka o reálnej veľkosti MAX_HT_SIZE
typedef ht_item_t *ht_table_t[MAX_HT_SIZE];

int get_hash(char *key);
void ht_init(ht_table_t *table);
ht_item_t *ht_search(ht_table_t *table, char *key);
void ht_insert(ht_table_t *table, char *key, float data);
float *ht_get(ht_table_t *table, char *key);
void ht_delete(ht_table_t *table, char *key);
void ht_delete_all(ht_table_t *table);

#endif
