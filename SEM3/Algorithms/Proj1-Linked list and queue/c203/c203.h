/* ******************************* c203.h *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Hlavičkový soubor pro c203.c (Fronta znaků v poli)                        */
/*  Úkol: c203 - Fronta znaků v poli                                          */
/*  Vytvořil: Václav Topinka, září 2005                                       */
/*  Úpravy: Kamil Jeřábek, září 2020                                          */
/*          Daniel Dolejška, září 2021                                        */
/* ************************************************************************** */

/* TENTO SOUBOR, PROSÍME, NEUPRAVUJTE! */

#ifndef C203_H
#define C203_H

#include <stdio.h>

#define FALSE 0
#define TRUE 1

/**
 * Hodnota MAX_QUEUE udává skutečnou velikost statického pole pro uložení hodnot
 * fronty. Při implementaci ADT fronta byste tedy hodnotu MAX_QUEUE neměli vůbec
 * použít.
 */
#define MAX_QUEUE 50

/**
 * Při implementaci operací nad ADT frontou předpokládejte, že velikost tohoto
 * pole je pouze QUEUE_SIZE. Umožní nám to jednoduše měnit velikost fronty
 * v průběhu testování.
 *
 * Pamatujte, že do fronty se vejde maximálně (QUEUE_SIZE - 1) prvků. Jedna
 * pozice ve frontě bude vždy nevyužitá, aby bylo možné odlišit prázdnou frontu
 * od plné.
 */
extern int QUEUE_SIZE;

/** Globální proměnná - indikuje, zda operace volala chybu. */
extern int error_flag;
/** Globální proměnná - indikuje, zda byla operace řešena. */
extern int solved;

/** Celkový počet možných chyb. */
#define MAX_QERR    5
/** Chyba při Queue_Enqueue. */
#define QERR_ENQUEUE     1
/** Chyba při Queue_Front. */
#define QERR_FRONT  2
/** Chyba při Queue_Remove. */
#define QERR_REMOVE 3
/** Chyba při Queue_Dequeue. */
#define QERR_DEQUEUE    4
/** Chyba při malloc. */
#define QERR_INIT   5

/** ADT fronta implementovaná ve statickém poli. */
typedef struct {
	/** Pole pro uložení hodnot. */
	char array[MAX_QUEUE];
	/** Index prvního prvku. */
	int firstIndex;
	/** Index první volné pozice. */
	int freeIndex;
} Queue;

void Queue_Error( int error_code );

void Queue_Init( Queue *queue );

int Queue_IsEmpty( const Queue *queue );

int Queue_IsFull( const Queue *queue );

void Queue_Front( const Queue *queue, char *dataPtr );

void Queue_Remove( Queue *queue );

void Queue_Dequeue( Queue *queue, char *dataPtr );

void Queue_Enqueue( Queue *queue, char data );

#endif

/* Konec hlavičkového souboru c203.h */
