/**
 * CS 2110 - Fall 2019 - Homework #9
 *
 * @author Benjamin Melnick
 *
 * list.c: Complete the functions!
 */

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!-IMPORTANT-!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * For any function that must use malloc, if malloc returns NULL, the function
 * itself should return NULL if needs to return something (or return 0 if
 * the function returns a int).
 */

// Do not add ANY additional includes!!!
#include "list.h"
#include <stdio.h>

/* You should NOT have any global variables. */

/* The create_node function is static because this is the only file that should
   have knowledge about the nodes backing the linked List. */
static inline Node *create_node(Pokemon *data);

/** create_node
  *
  * Helper function that creates a Node by allocating memory for it on the heap.
  * Be sure to set its next pointer to NULL.
  *
  * If malloc returns NULL, you should return NULL to indicate failure.
  *
  * @param data a void pointer to data the user wants to store in List
  * @return a Node
  */
static Node* create_node(Pokemon *data)
{
  Node* node = (Node*) malloc(sizeof(Node));
  if (node == NULL) {
    return NULL;
  }
  node->data = data;
  node->next = NULL;
  return node;
}

/** create_list
  *
  * Creates a struct linked_list by allocating memory for it on the heap.
  * Be sure to initialize size to zero and head and tail to NULL.
  *
  * If malloc returns NULL, you should return NULL to indicate failure.
  *
  * @return a pointer to a new struct list or NULL on failure
  */
LinkedList *create_list(void)
{
  LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
  if (list == NULL) {
    return NULL;
  }
  list->head = NULL;
  list->size = 0;
  return list;
}

/** push_front
  *
  * Adds the element at the front of the struct linked_list.
  *
  * @param listToPrependTo a pointer to the struct linked_list structure.
  * @param data the data to add to the struct linked_list.
  * @return 0 if the struct linked_list is NULL, 1 if successful.
  */
int push_front(LinkedList *listToPrependTo, Pokemon *data)
{
  if (listToPrependTo == NULL) {
    return 0;
  }
  Node* head = listToPrependTo->head;
  Node* newNode = create_node(data);
  if (newNode == NULL) {
    // attempting to add a new node failed
    free(newNode);
    return 0;
  }
  if (head == NULL) {
      // head is null, assign new node to head
      listToPrependTo->head = newNode;
    } else {
      // head is not null, move head over and make new node the head
      newNode->next = head;
      listToPrependTo->head = newNode;
    }
  listToPrependTo->size += 1;
  return 1;
}

/** push_back
  *
  * Adds the element to the back of the LinkedList.
  *
  * @param listToAppendTo a pointer to the LinkedList structure.
  * @param data the data to add to the LinkedList.
  * @return 0 if the LinkedList is NULL, 1 if successful.
  */
int push_back(LinkedList *listToAppendTo, Pokemon *data)
{
  if (listToAppendTo == NULL) {
    return 0;
  }
  Node* head = listToAppendTo->head;
  Node* newNode = create_node(data);
  if (newNode == NULL) {
    // attempting to add a new node failed
    free(newNode);
    return 0;
  }
  if (head == NULL) {
      // head is null, assign new node to head
      listToAppendTo->head = newNode;
    } else {
      // head is not null, move head over and make new node the head
      head->next = newNode;
    }
  listToAppendTo->size += 1;
  return 1;

}

/** add_at_index
  *
  * Add the element at the specified index in the LinkedList. This index must lie in
  * the inclusive range [0,size].
  * For example, if you have no elements in the LinkedList,
  * you should be able to add to index 0 but no further.
  * If you have two elements in the LinkedList,
  * you should be able to add to index 2 but no further.
  *
  * @param listToAddTo a pointer to the LinkedList structure
  * @param index 0-based, starting from the head in the inclusive range
  *              [0,size]
  * @param data the data to add to the LinkedList
  * @return 0 if the index is out of bounds or the LinkedList is NULL
  *         (do not add the data in this case)
  *         otherwise return 1
  */
int add_at_index(LinkedList *listToAddTo, Pokemon *data, int index)
{
  if (listToAddTo == NULL) {
    return 0;
  }
  if (index < 0 || index > listToAddTo->size) {
    //index out of boudns
    return 0;
  }
  if (index == 0) {
    // add to front
    return push_front(listToAddTo, data);
  } else if (index == listToAddTo->size) {
    //add at the back
    return push_back(listToAddTo, data);
  }
  // adding in b/w the front and back of the list
  Node* curr = listToAddTo->head;
  Node* newNode = create_node(data);
  if (newNode == NULL) {
    // attempting to add a new node failed
    free(newNode);
    return 0;
  }
  // get the node just before the index we are adding at
  for (int i = 0; i < index - 1; i += 1) {
    curr = curr->next;
  }
  Node* temp = curr->next;  // node occupying the index we want to add to
  newNode->next = temp;
  curr->next = newNode;
  listToAddTo->size += 1;
  return 1;
}

/** get
  *
  * Gets the data at the specified index in the LinkedList
  *
  * @param listToGetDataFrom a pointer to the LinkedList structure
  * @param index 0-based, starting from the head.
  * @param dataOut A pointer to a pointer used to return the data from the
  *        specified index in the LinkedList or NULL on failure.
  * @return 0 if dataOut is NULL or index is out of range of the LinkedList or
  *         the LinkedList is NULL, 1 otherwise
  */
int get(LinkedList *listToGetDataFrom, int index, Pokemon **dataOut)
{

  if (listToGetDataFrom == NULL) {
    if (dataOut != NULL) {
      *dataOut = NULL;
    }
    return 0;
  }
  if (dataOut == NULL) {
    return 0;
  }
  if (index < 0 || index >= listToGetDataFrom->size) {
    // provided index is out of range of the list
    if (dataOut != NULL) {
      *dataOut = NULL;
    }
    return 0;
  }
  Node* curr = listToGetDataFrom->head;
  for (int i = 0; i < index; i += 1) {
    curr = curr->next;
  }
  *dataOut = curr->data;
  return 1;
}

/** contains
  *
  * Traverses the LinkedList, trying to see if the LinkedList contains some
  * data. We will consider a node to contain the same data if the pokemon at that
  * node has the same name as the pokemon passed in.
  *
  * The "data" parameter may not necessarily point to the same address as the
  * equal data you are returning from this function, it's just data which the
  * comparator (pokemon_eq_name in this case) says is equal.
  *
  * If there are multiple pieces of data in the LinkedList which are equal to
  * the "data" parameter, return the one at the lowest index.
  *
  * Use pokemon_eq_name to compare pokemon structs. Be sure to take a look at that
  * function before you use it! Also be sure not to use pokemon_eq.
  *
  * @param listToSearch a pointer to the LinkedList structure
  * @param data The data, to see if it exists in the LinkedList
  * @param dataOut A pointer to a pointer used to return the data contained in
  *                the LinkedList or NULL on failure
  * @return int    0 if dataOut is NULL, the list is NULL, or the list
  *                does not contain data, else 1
  */
int contains(LinkedList *listToSearch, Pokemon *data, Pokemon **dataOut)
{
  if (listToSearch == NULL) {
    if (dataOut != NULL) {
      *dataOut = NULL;
    }
    return 0;
  }
  if (dataOut == NULL) {
    return 0;
  }
  Node* curr = listToSearch->head;
  for (int i = 0; i < listToSearch->size - 1; i += 1) {
    Pokemon* pokemon = curr->data;
    if (pokemon_eq_name(data, pokemon) == 0) {
      // return the first instance matching the passed in name
      *dataOut = pokemon;
      return 1;
    }
    curr = curr->next;
  }
  // didn't find any matches
  if (dataOut != NULL) {
    *dataOut = NULL;
  }
  return 0;
}

/** pop_front
  *
  * Removes the List_node at the front of the LinkedList, and returns its data to the user
  *
  * @param listToPopFrom a pointer to the LinkedList.
  * @param dataOut A pointer to a pointer used to return the data in the first
  *                List_node or NULL on failure
  * @return 0 if dataOut is NULL or the LinkedList is NULL or empty, else 1
  */
int pop_front(LinkedList *listToPopFrom, Pokemon **dataOut)
{
  if (listToPopFrom == NULL) {
    if (dataOut != NULL) {
      *dataOut = NULL;
    }
    return 0;
  }
  if (dataOut == NULL) {
    return 0;
  }
  if (listToPopFrom->size == 0) {
    //empty list
    if (dataOut != NULL) {
      *dataOut = NULL;
    }
    return 0;
  }
  Node* removed = listToPopFrom->head;
  *dataOut = removed->data;
  if (listToPopFrom->size == 1) {
    //only one node to remove, set head to NULL
    listToPopFrom->head = NULL;
  } else {
    //head = head.next
    listToPopFrom->head = removed->next;
  }
  free(removed);
  listToPopFrom->size -= 1;
  return 1;
}

/** pop_back
  *
  * Removes the ListNode at the back of the LinkedList, and returns its data to the user
  *
  * @param listToPopFrom a pointer to the LinkedList.
  * @param dataOut A pointer to a pointer used to return the data in the last
  *                ListNode or NULL on failure
  * @return 0 if dataOut is NULL or the LinkedList is NULL or empty, else 1
  */
int pop_back(LinkedList *listToPopFrom, Pokemon **dataOut)
{

  if (listToPopFrom == NULL) {
    if (dataOut != NULL) {
      *dataOut = NULL;
    }
    return 0;
  }
  if (dataOut == NULL) {
    return 0;
  }
  if (listToPopFrom->size == 0) {
    //empty list
    if (dataOut != NULL) {
      *dataOut = NULL;
    }
    return 0;
  }
  if (listToPopFrom->size == 1) {
    // only one node to remove, take the head
    Node* removed = listToPopFrom->head;
    *dataOut = removed->data;
    free(removed);
    listToPopFrom->head = NULL;
  } else {
    // need to iterate to the back of the list
    // get the node just before the last node
    Node* curr = listToPopFrom->head;
    int index = listToPopFrom->size - 1; // index of node we are removing
    for (int i = 0; i < index - 1; i += 1) {
      curr = curr->next;
    }
    Node* removed = curr->next;
    if (removed == NULL) {
      printf("NULL\n");
    }
    *dataOut = removed->data;
    curr->next = NULL;
    free(removed);
  }
  listToPopFrom->size -= 1;
  return 1;
}


/** remove_at_index
  *
  * Remove the element at the specified index in the LinkedList.
  *
  * @param listToAddTo a pointer to the LinkedList structure
  * @param index 0-based, starting from the head in the inclusive range
  *              [0,size]
  * @param data the data to add to the LinkedList
  * @return 0 if the index is out of bounds, the LinkedList is NULL or
  *         the dataOut is NULL
  *         otherwise return 1
  */
int remove_at_index(LinkedList * listToRemoveFrom, Pokemon ** dataOut, int index)
{
  if (listToRemoveFrom == NULL) {
    if (dataOut != NULL) {
      *dataOut = NULL;
    }
    return 0;
  }
  if (dataOut == NULL) {
    return 0;
  }
  if (index < 0 || index >= listToRemoveFrom->size) {
    //index out of bounds
    if (dataOut != NULL) {
      *dataOut = NULL;
    }
    return 0;
  }
  if (index == 0) {
    return pop_front(listToRemoveFrom, dataOut);
  } else if (index == listToRemoveFrom->size - 1) {
    return pop_back(listToRemoveFrom, dataOut);
  } else {
    // removing in b/w front and end of list
    // get node right in front of index to remove at
    Node* curr = listToRemoveFrom->head;
    for (int i = 0; i < index - 1; i += 1) {
      curr = curr->next;
    }
    Node* removed = curr->next;
    *dataOut = removed->data;
    Node* afterRemoved = removed->next;
    //link curr and afterRemoved
    curr->next = afterRemoved;
    free(removed);
  }
  listToRemoveFrom->size -= 1;
  return 1;
}

/** empty_list
  *
  * Empties the LinkedList. After this is called, the LinkedList should be
  * empty. This does not free the LinkedList struct itself, just all nodes and
  * data within. Make sure to check that listToEmpty is not NULL before
  * using it.
  *
  * Call pokemon_free to free a pokemon struct.
  *
  * @param listToEmpty a pointer to the LinkedList structure
  */
void empty_list(LinkedList *listToEmpty)
{
  if (listToEmpty != NULL) {
    Node* curr = listToEmpty->head;
    while (curr != NULL) {
      Node* temp = curr;
      curr = curr->next;
      pokemon_free(temp->data);
      free(temp);
    }
    listToEmpty->head = NULL;
    listToEmpty->size = 0;
  }
}

/** swap_pokemon
  *
  *
  * Swaps two nodes in the LinkedList. After this is called, the nodes containing the
  * this Pokemon data should be swapped. Make sure you swap the nodes themselves
  * and NOT just the data!
  * Consider all cases! (nodes being side by side, one of the nodes being the head, etc.)
  *
  *     LinkedList *A: A1 -> A2 -> A3 -> A4 -> A5 -> NULL
  *     swap_pokemon(A, A2, A4);
  *     LinkedList *A: A1 -> A4 -> A3 -> A2 -> A5 -> NULL
  *
  * @param poke1 first node to swap
  * @param poke2 second node to swap
  * @return 0 if Pokemon not in list or are NULL or if list is NULL, 1 on successful swap
  */
int swap_pokemon(LinkedList *list, Node *poke1, Node *poke2)
{
  if (list == NULL) {
    return 0;
  }
  if (poke1 == NULL || poke2 == NULL) {
    return 0;
  }
  // search for poke1 and the node just before poke1
  Node* prev1 = NULL;
  Node* curr1 = list->head;
  while (curr1 != NULL && pokemon_eq(curr1->data, poke1->data) == 0) {
    prev1 = curr1;
    curr1 = curr1->next;
  }

  //search for poke2 and the node just before poke2
  Node* prev2 = NULL;
  Node* curr2 = list->head;
  while (curr2 != NULL && pokemon_eq(curr2->data, poke2->data) == 0) {
    prev2 = curr2;
    curr2 = curr2->next;
  }

  if (curr1 == NULL || curr2 == NULL) {
    // either pokemon is not present
    return 0;
  }

  if (prev1 != NULL) {
    // poke1 is not the head of the list
    // perform swap - place poke2 where poke1 used to be
    prev1->next = curr2;
  } else {
    // poke1 is the head of the list, make poke2 new head
    list->head = curr2;
  }

  if (prev2 != NULL) {
    // poke2 is not the head of the list
    // perform swap - place poke1 where poke2 used to be
    prev2->next = curr1;
  } else {
    // poke2 is the head, make poke1 new head
    list->head = curr1;
  }
  //swap next pointers
  Node* temp = curr2->next;
  curr2->next = curr1->next;
  curr1->next = temp;
  return 1;
}

/** zip
  *
  *
  * Zips two Lists together. After this is called, the first list should
  * have the elements from both lists and the second list should be empty.
  * Do not free the second list. Both Lists might be empty. If either list
  * is NULL, just return.
  *
  *     LinkedList *A: A1 -> A2 -> A3 -> A4 -> A5 -> NULL
  *
  *     LinkedList *B: B1 -> B2 -> B3 -> NULL
  *
  *     zip(A, B): A1 -> B1 -> A2 -> B2 -> A3 -> B3 -> A4 -> A5 -> NULL
  *
  * @param team1 first LinkedList to zip together
  * @param team2 second LinkedList to zip together
  */
void zip(LinkedList *team1, LinkedList *team2)
{
  if (team1 == NULL || team2 == NULL) {
    return;
  }
  if (team2->size == 0) {
    // second list empty, no zipping necessary, just return
    return;
  }
  if (team1->size == 0) {
    // team 1 is empty - copy team2 over to team1
    team1->head = team2->head;
    team1->size = team2->size;
    // empty list - but don't free
    team2->head = NULL;
    team2->size = 0;
    return;
  }
  Node* curr1 = team1->head;
  Node* curr2 = team2->head;
  while (curr1 != NULL && curr2 != NULL) {
    Node* currNext1 = curr1->next;
    Node* currNext2 = curr2->next;
    curr2->next = currNext1;
    curr1->next = curr2;
    curr1 = currNext1;
    curr2 = currNext2;
  }
  team1->size += team2->size;
  // empty list - but don't free
  team2->head = NULL;
  team2->size = 0;
}
