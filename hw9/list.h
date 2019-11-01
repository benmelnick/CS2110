/**
 * CS 2110 - Fall 2019 - Homework #9
 *
 * Do not modify this file!
 *
 * struct list.h
 */

#ifndef LIST_H
#define LIST_H

#include "pokemon.h"
#include <stdlib.h>
#include <string.h>

// This is just to remove compiler warnings associated with unused variables.
// Delete calls to this as you implement functions.
#define UNUSED_PARAMETER(x) (void)(x)

/**************************
 ** Datatype definitions **
 **************************/

/*
 * The following structs define the LinkedList and Node for use in list.c. DO NOT MODIFY THESE STRUCT DEFINITIONS
 */

typedef struct node
{
    struct node *next;
    struct pokemon *data;
} Node;


typedef struct linked_list
{
    Node *head; // Head pointer either points to a node with data or if the struct list is empty NULL
    int size;   // Size of the struct list
} LinkedList;

/***************************************************
** Prototypes for struct list library functions.  **
**                                                **
** For more details on their functionality,       **
** check struct list.c.                           **
***************************************************/

/* Creating */
LinkedList *create_list(void);

/* Adding */
int push_front(LinkedList *, Pokemon *);
int push_back(LinkedList *, Pokemon *);
int add_at_index(LinkedList *, Pokemon *, int index);

/* Querying */
int get(LinkedList *, int, Pokemon **);
int contains(LinkedList *, Pokemon *, Pokemon **);

/* Removing */
int pop_front(struct linked_list *, Pokemon **);
int pop_back(struct linked_list *, Pokemon **);
int remove_at_index(LinkedList *, Pokemon **, int index);
void empty_list(LinkedList *);

/* Mutating */
int swap_pokemon(LinkedList *, Node *, Node *);
void zip(LinkedList *, LinkedList *);

#endif
