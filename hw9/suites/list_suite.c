// Homework 9 Tests, Fall 20119
// Structure by Austin, actual work by Jim and Sanjay (RIP)
//
// Warning: much the structure of this file is shamelessly copypasted from
// https://libcheck.github.io/check/doc/check_html/check_3.html

#include <stdio.h>
#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "test_utils.h"

#define TESTING_SWAP_TESTS 1

static struct linked_list *l;

// The number of mallocs in user code until the next one fails, or -1 for
// successful mallocs
int mallocs_until_fail = -1;

/**
 * @brief Print list
 * 
 * @param l1 
 * @param before_after 0 before 1 after
 */
void print_list(LinkedList* l1, int before_after){
    if(!l1) return;

    Node* n = l1->head;
    if(!n) return;
    while(n->next){
        printf("%s --> ", n->data->name);
        n = n->next;
    }
    if(!before_after)
        printf("%s\n", n->data->name);
    else printf("%s\n\n", n->data->name);
}

int pokemon_name_is(Pokemon *pokemon, const char* name) {
    return strcmp(pokemon->name, name) == 0;
}

static void reset_mallocs_until_fail(void) {
    mallocs_until_fail = -1;
}

static struct linked_list *_create_list_size_1(void) {
    struct linked_list *longerList = _create_list();
    _push_front(longerList, pokemon_new("Charmander", "Fire", 1));
    return longerList;
}

static struct linked_list *_create_list_size_2(void) {
    struct linked_list *longerList = _create_list();
    _push_front(longerList, pokemon_new("Charmander", "Fire", 1));
    _push_front(longerList, pokemon_new("Pikachu", "Electric", 2));
    return longerList;
}

static struct linked_list *_create_longer_list2(void) {
    struct linked_list *longerList = _create_list();
    _push_front(longerList, pokemon_new("Charmander", "Fire", 1));
    _push_front(longerList, pokemon_new("Squirtle", "Water", 4));
    _push_front(longerList, pokemon_new("Pikachu", "Electric", 2));
    return longerList;
}

// this function was used for sort() last year. leaving it here just in case

static struct linked_list *_create_even_longer_list(void) {
    struct linked_list *longerList = _create_list();
    _push_front(longerList, pokemon_new("Charmander", "Fire", 1));
    _push_front(longerList, pokemon_new("Squirtle", "Water", 4));
    _push_front(longerList, pokemon_new("Pikachu", "Electric", 5));
    _push_front(longerList, pokemon_new("Eevee", "Normal", 3));
    _push_front(longerList, pokemon_new("Dratini", "Dragon", 8));
    _push_front(longerList, pokemon_new("Magikarp", "Water", 2));
    return longerList;
}

void teardown_bar_list(void) {
    if (l != NULL) {
        _free_list_nodes_and_data(l, pokemon_free);
        l = NULL;
    }
}

#if TESTING_SWAP_TESTS

static struct linked_list *_create_length_1_list(void) {
    struct linked_list *shortList = _create_list();
    _push_front(shortList, pokemon_new("Squirtle", "Water", 13));
    return shortList;
}

static struct linked_list *_create_longer_list(void) {
    struct linked_list *longerList = _create_list();
    _push_front(longerList, pokemon_new("Dratini", "Dragon", 7));
    _push_front(longerList, pokemon_new("Squirtle", "Water", 13));
    _push_front(longerList, pokemon_new("Eevee", "Pointer", 16));
    return longerList;
}

/******************************************************************************/
/*************************** create_list() tests ******************************/
/******************************************************************************/
START_TEST(test_list_create_list_basic) {
    struct linked_list *l1 = create_list();
    ck_assert(l1);
    ck_assert(!l1->size);
    ck_assert(!l1->head);
    free(l1);
}
END_TEST

START_TEST(test_list_create_list_malloc_failure) {
    mallocs_until_fail = 0;
    struct linked_list *l1 = create_list();
    ck_assert(!l1);
}
END_TEST

/******************************************************************************/
/************************** push_front() tests ********************************/
/******************************************************************************/
void setup_list_push_front(void) {
    reset_mallocs_until_fail();
    l = _create_list();
    ck_assert(l);
}

void teardown_list_push_front(void) {
    if (l) {
        free(l);
        l = NULL;
    }
}

START_TEST(test_list_push_front_NULL_list) {
    struct pokemon *b = pokemon_new("Squirtle", "Water", 13);
    ck_assert(!push_front(NULL, b));
    pokemon_free(b);
}
END_TEST

START_TEST(test_list_push_front_NULL_data) {
    ck_assert(push_front(l, NULL));
    ck_assert_int_eq(l->size, 1);
    ck_assert(l->head);
    ck_assert(!l->head->next);
    ck_assert(!l->head->data);

    free(l->head);
}
END_TEST

START_TEST(test_list_push_front_empty) {
    struct pokemon *b = pokemon_new("Henry", "Normal", 21);
    ck_assert(push_front(l, b));
    ck_assert_int_eq(l->size, 1);
    ck_assert(l->head);
    ck_assert(!l->head->next);
    ck_assert_ptr_eq(l->head->data, b);

    // Don't use empty_list() because we don't want to nail everyone for
    // memory leaks if it's broken
    free(l->head);
    pokemon_free(b);
}
END_TEST

START_TEST(test_list_push_front_nonempty) {
    // Put some stuff in the struct linked_list
    struct pokemon *b0 = pokemon_new("Pikachu", "Ice", 1);
    struct pokemon *b1 = pokemon_new("Pikachu", "Water", 2);
    struct pokemon *b2 = pokemon_new("Pikachu", "Fire", 3);
    l->head = _create_node(b0);
    l->head->next = _create_node(b1);
    l->size = 3;

    struct pokemon *b3 = pokemon_new("Pikachu", "Grass", 4);
    ck_assert(push_front(l, b3));
    ck_assert_int_eq(l->size, 4);
    ck_assert(l->head);
    ck_assert_ptr_eq(l->head->data, b3);

    // Don't use empty_list() because we don't want to nail everyone for
    // memory leaks if it's broken
    free(l->head->next->next->next);
    free(l->head->next->next);
    free(l->head->next);
    free(l->head);
    pokemon_free(b0);
    pokemon_free(b1);
    pokemon_free(b2);
    pokemon_free(b3);
}
END_TEST

/******************************************************************************/
/************************** push_back() tests *********************************/
/******************************************************************************/
START_TEST(test_list_push_back_empty) {
    struct linked_list *l1 = _create_list();
    struct pokemon *b0 = pokemon_new("Evee", "Dragon", 60);
    ck_assert(push_back(l1, b0));
    ck_assert_ptr_eq(l1->head->data, b0);
    ck_assert_int_eq(l1->size, 1);
    free(l1->head);
    free(l1);
    pokemon_free(b0);

}
END_TEST
START_TEST(test_list_push_back_non_empty) {
    struct linked_list *l1 = _create_list();
    struct pokemon *b0 = pokemon_new("Bulbasaur", "Steel", 10);
    struct pokemon *b1 = pokemon_new("Squirtle", "Grass", 10);

    _push_front(l1, b0);

    ck_assert(push_back(l1, b1));
    ck_assert_ptr_eq(b0, l1->head->data);
    ck_assert_int_eq(l1->size, 2);

    free(l1->head->next);
    free(l1->head);
    pokemon_free(b0);
    pokemon_free(b1);
    free(l1);
}
END_TEST
START_TEST(test_list_push_back_null) {
    struct linked_list *l1 = _create_list();
    struct pokemon *b0 = pokemon_new("Pichu", "Dragon", 10);
    ck_assert(!push_back(NULL, b0));
    ck_assert(push_back(l1, NULL));
    ck_assert_int_eq(l1->size, 1);
    ck_assert(!l1->head->data);

    pokemon_free(b0);
    free(l1->head);
    free(l1);
}
END_TEST


/******************************************************************************/
/************************** add_at_index() tests ***************************************/
/******************************************************************************/
START_TEST(test_list_add_at_index_out_of_bounds) {
    struct linked_list *l1 = _create_list();
    struct pokemon *b0 = pokemon_new("Squirtle", "Electric", 10);

    ck_assert(!add_at_index(l1, b0, -1));

    _push_front(l1, b0);

    ck_assert(!add_at_index(l1, b0, 2));

    free(l1->head);
    free(l1);
    pokemon_free(b0);

}
END_TEST

START_TEST(test_list_add_at_index_empty_list) {
    struct linked_list *l1 = _create_list();
    struct pokemon *b0 = pokemon_new("Mawile", "Steel", 1);
    struct pokemon *b1 = pokemon_new("Charmander", "Fairy", 1);

    ck_assert(add_at_index(l1, b0, 0));
    ck_assert_int_eq(l1->size, 1);
    ck_assert_ptr_eq(l1->head->data, b0);

    ck_assert(!add_at_index(l1, b1, 2));

    ck_assert(add_at_index(l1, b1, 1));

    ck_assert_int_eq(l1->size, 2);

    _free_list_nodes_and_data(l1, pokemon_free);


}
END_TEST

START_TEST(test_list_add_at_index_non_empty_list) {

    struct linked_list *l1 = _create_longer_list();
    struct pokemon *b0 = pokemon_new("Pikachu", "Steel", 1);
    ck_assert(add_at_index(l1, b0, 1));
    ck_assert_ptr_eq(l1->head->next->data, b0);
    ck_assert_int_eq(l1->size, 4);

    _free_list_nodes_and_data(l1, pokemon_free);

}
END_TEST

START_TEST(test_list_add_at_index_null) {
    struct linked_list *l1 = _create_list();
    struct pokemon *b0 = pokemon_new("Pikachu", "Rock", 1);

    ck_assert(!add_at_index(NULL, NULL, 0));

    ck_assert(!add_at_index(NULL, b0, 0));

    ck_assert(add_at_index(l1, NULL, 0));
    ck_assert_int_eq(l1->size, 1);
    ck_assert(!l1->head->data);

    free(l1->head);
    free(l1);
    pokemon_free(b0);
}
END_TEST
/******************************************************************************/
/*************************** pop_front() tests ********************************/
/******************************************************************************/
START_TEST(test_list_pop_front_NULL_list) {
    struct pokemon *dataOut = (struct pokemon *) 0xDEADBEEFU;
    ck_assert(!pop_front(NULL, &dataOut));
}
END_TEST

START_TEST(test_list_pop_front_NULL_dataOut) {
    l = _create_list();
    ck_assert(!pop_front(l, NULL));
}
END_TEST

START_TEST(test_list_pop_front_empty) {
    l = _create_list();
    struct pokemon *dataOut = (struct pokemon *) 0xDEADBEEFU;
    ck_assert(!pop_front(l, &dataOut));
}
END_TEST

START_TEST(test_list_pop_front_nonempty) {
    l = _create_length_1_list();
    struct pokemon *Squirtle = NULL;
    int result = pop_front(l, &Squirtle);
    ck_assert(Squirtle);
    int nameCheck = pokemon_name_is(Squirtle, "Squirtle");
    pokemon_free(Squirtle);
    ck_assert(result);
    ck_assert(nameCheck);
    ck_assert_int_eq(l->size, 0);
}
END_TEST

START_TEST(test_list_pop_front_longer_list) {
    l = _create_longer_list();
    struct pokemon *data = NULL;
    int result = pop_front(l, &data);
    ck_assert(data);
    int nameCheck = pokemon_name_is(data, "Eevee");
    pokemon_free(data);
    ck_assert(result);
    ck_assert(nameCheck);
    ck_assert_int_eq(l->size, 2);
    ck_assert(pokemon_name_is(l->head->data, "Squirtle"));
    ck_assert(pokemon_name_is(l->head->next->data, "Dratini"));
    ck_assert(!l->head->next->next);

    result = pop_front(l, &data);
    nameCheck = pokemon_name_is(data, "Squirtle");
    pokemon_free(data);
    ck_assert(result);
    ck_assert(nameCheck);
    ck_assert_int_eq(l->size, 1);
    ck_assert(pokemon_name_is(l->head->data, "Dratini"));
    ck_assert(!l->head->next);
}
END_TEST

/******************************************************************************/
/**************************** pop_back() tests ********************************/
/******************************************************************************/
START_TEST(test_list_pop_back_NULL_list) {
    struct pokemon *dataOut = (struct pokemon *)0xDEADBEEFU;
    ck_assert(!pop_back(NULL, &dataOut));
}
END_TEST

START_TEST(test_list_pop_back_NULL_dataOut) {
    l = _create_list();
    ck_assert(!pop_back(l, NULL));
}
END_TEST

START_TEST(test_list_pop_back_empty) {
    l = _create_list();
    struct pokemon *dataOut = (struct pokemon *)0xDEADBEEFU;
    ck_assert(!pop_back(l, &dataOut));
}
END_TEST

START_TEST(test_list_pop_back_nonempty) {
    l = _create_length_1_list();
    struct pokemon *Squirtle = NULL;
    int result = pop_back(l, (struct pokemon **) &Squirtle);
    ck_assert(Squirtle);

    int nameCheck = pokemon_name_is(Squirtle, "Squirtle");
    pokemon_free(Squirtle);
    ck_assert(result);
    ck_assert(nameCheck);
    ck_assert_int_eq(l->size, 0);
}
END_TEST

START_TEST(test_list_pop_back_longer_list) {
    l = _create_longer_list();
    struct pokemon *data = NULL;
    int result = pop_back(l, (struct pokemon **) &data);
    ck_assert(data);
    int name_check = pokemon_name_is(data, "Dratini");
    pokemon_free(data);
    ck_assert(result);
    ck_assert(name_check);
    ck_assert_int_eq(l->size, 2);
    ck_assert(pokemon_name_is(l->head->data, "Eevee"));
    ck_assert(pokemon_name_is(l->head->next->data, "Squirtle"));
    ck_assert(!l->head->next->next);

    result = pop_back(l, (struct pokemon **) &data);
    name_check = pokemon_name_is(data, "Squirtle");
    pokemon_free(data);
    ck_assert(result);
    ck_assert(name_check);
    ck_assert_int_eq(l->size, 1);
    ck_assert(pokemon_name_is(l->head->data, "Eevee"));
    ck_assert(!l->head->next);
}
END_TEST

/******************************************************************************/
/**************************** remove_at_index() tests *****************************/
/******************************************************************************/
START_TEST(test_list_remove_at_index_NULL_list) {
    struct pokemon *dataOut = (struct pokemon *) 0xDEADBEEFU;
    ck_assert(!remove_at_index(NULL, &dataOut, 0));
}
END_TEST

START_TEST(test_list_remove_at_index_NULL_dataOut) {
    l = _create_length_1_list();
    ck_assert(!remove_at_index(l, NULL, 0));
}
END_TEST

START_TEST(test_list_remove_at_index_out_of_bounds) {
    l = _create_length_1_list();
    ck_assert(!remove_at_index(l, NULL, -1));
}
END_TEST

START_TEST(test_list_remove_at_index_front) {
    l = _create_longer_list();
    struct pokemon *dataOut = NULL;
    int result = remove_at_index(l, &dataOut, 0);
    ck_assert(dataOut);
    int nameCheck = pokemon_name_is(dataOut, "Eevee");
    pokemon_free(dataOut);
    ck_assert(result);
    ck_assert(nameCheck);
    ck_assert_int_eq(l->size, 2);
    ck_assert(pokemon_name_is(l->head->data, "Squirtle"));
    ck_assert(pokemon_name_is(l->head->next->data, "Dratini"));
}
END_TEST

START_TEST(test_list_remove_at_index_middle) {
    l = _create_longer_list();
    struct pokemon *dataOut = NULL;
    int result = remove_at_index(l, &dataOut, 1);
    ck_assert(dataOut);
    int nameCheck = pokemon_name_is(dataOut, "Squirtle");
    pokemon_free(dataOut);
    ck_assert(result);
    ck_assert(nameCheck);
    ck_assert_int_eq(l->size, 2);
    ck_assert(pokemon_name_is(l->head->data, "Eevee"));
    ck_assert(pokemon_name_is(l->head->next->data, "Dratini"));
    ck_assert(!l->head->next->next);
}
END_TEST

START_TEST(test_list_remove_at_index_back) {
    l = _create_longer_list();
    struct pokemon *dataOut = NULL;
    int result = remove_at_index(l, &dataOut, 2);
    ck_assert(dataOut);
    int nameCheck = pokemon_name_is(dataOut, "Dratini");
    pokemon_free(dataOut);
    ck_assert(result);
    ck_assert(nameCheck);
    ck_assert_int_eq(l->size, 2);
    ck_assert(pokemon_name_is(l->head->data, "Eevee"));
    ck_assert(pokemon_name_is(l->head->next->data, "Squirtle"));
    ck_assert(!l->head->next->next);
}
END_TEST

START_TEST(test_list_remove_at_index_length_1) {
    l = _create_length_1_list();
    struct pokemon *dataOut = NULL;
    int result = remove_at_index(l, &dataOut, 0);
    ck_assert(dataOut);
    int nameCheck = pokemon_name_is(dataOut, "Squirtle");
    pokemon_free(dataOut);
    ck_assert(result);
    ck_assert(nameCheck);
    ck_assert_int_eq(l->size, 0);
    ck_assert(!l->head);
}
END_TEST

/******************************************************************************/
/************************** empty_list() tests ********************************/
/******************************************************************************/
START_TEST(test_list_empty_list_NULL_list) {
    // Success if there is no error
    empty_list(NULL);
}
END_TEST

START_TEST(test_list_empty_list_empty) {
    l = _create_list();
    empty_list(l);
    ck_assert_int_eq(l->size, 0);
}
END_TEST

START_TEST(test_list_empty_list_length_1) {
    l = _create_length_1_list();
    empty_list(l);
    ck_assert_int_eq(l->size, 0);
}
END_TEST

START_TEST(test_list_empty_list_longer_list) {
    l = _create_longer_list();
    empty_list(l);
    ck_assert_int_eq(l->size, 0);
}
END_TEST

/******************************************************************************/
/******************************* get() tests **********************************/
/******************************************************************************/
void checkLongerListUnchanged(void) {
    ck_assert(pokemon_name_is(l->head->data, "Eevee"));
    ck_assert(pokemon_name_is(l->head->next->data, "Squirtle"));
    ck_assert(pokemon_name_is(l->head->next->next->data, "Dratini"));
    ck_assert(!l->head->next->next->next);
}

START_TEST(test_list_get_NULL_list) {
    struct pokemon *b = (struct pokemon *)0xDEADBEEFU;
    ck_assert(!get(NULL, 0, &b));
}
END_TEST

START_TEST(test_list_get_NULL_dataOut) {
    l = _create_longer_list();
    ck_assert(!get(l, 0, NULL));
    checkLongerListUnchanged();
}
END_TEST

START_TEST(test_list_get_negative_index) {
    l = _create_longer_list();
    struct pokemon *b = (struct pokemon *) 0xDEADBEEFU;
    ck_assert(!get(l, -1, &b));
    checkLongerListUnchanged();
}
END_TEST

START_TEST(test_list_get_front) {
    l = _create_longer_list();
    struct pokemon *b = (struct pokemon *) 0xDEADBEEFU;
    ck_assert(get(l, 0, &b));
    ck_assert(pokemon_name_is(b, "Eevee"));
    checkLongerListUnchanged();
}
END_TEST

START_TEST(test_list_get_middle) {
    l = _create_longer_list();
    struct pokemon *b = (struct pokemon *) 0xDEADBEEFU;
    ck_assert(get(l, 1, &b));
    ck_assert(pokemon_name_is(b, "Squirtle"));
    checkLongerListUnchanged();
}
END_TEST

START_TEST(test_list_get_back) {
    l = _create_longer_list();
    struct pokemon *b = (struct pokemon *)0xDEADBEEFU;
    ck_assert(get(l, 2, &b));
    ck_assert(pokemon_name_is(b, "Dratini"));
    checkLongerListUnchanged();
}
END_TEST

START_TEST(test_list_get_out_of_bounds_index) {
    l = _create_longer_list();
    struct pokemon *b = (struct pokemon *) 0xDEADBEEFU;
    ck_assert(!get(l, 3, &b));
    checkLongerListUnchanged();
}
END_TEST

/******************************************************************************/
/**************************** contains() tests ********************************/
/******************************************************************************/

START_TEST(test_list_contains_NULL_list) {
    struct pokemon *henry = pokemon_new("Henry", "Normal", 21);
    struct pokemon *dataOut = (struct pokemon *) 0xDEADBEEFU;
    int result = contains(NULL, (struct pokemon *) henry, &dataOut);
    pokemon_free(henry);
    ck_assert(!result);
}
END_TEST

START_TEST(test_list_contains_NULL_dataOut) {
    struct pokemon *henry = pokemon_new("Henry", "Normal", 21);
    l = _create_longer_list();
    int result = contains(l, (struct pokemon *) henry, NULL);
    pokemon_free(henry);
    ck_assert(!result);
    checkLongerListUnchanged();
}
END_TEST

START_TEST(test_list_contains_empty) {
    l = _create_list();
    struct pokemon *henry = pokemon_new("Henry", "Normal", 21);
    struct pokemon *dataOut = (struct pokemon *) 0xDEADBEEFU;
    int result = contains(l, (struct pokemon *) henry, &dataOut);
    pokemon_free(henry);
    ck_assert(!result);
    ck_assert(!dataOut);
}
END_TEST

START_TEST(test_list_contains_not_contained) {
    l = _create_longer_list();
    struct pokemon *henry = pokemon_new("Marlet", "Water", 13);
    struct pokemon *dataOut = (struct pokemon *)0xDEADBEEFU;
    int result = contains(l, (struct pokemon *) henry, &dataOut);
    pokemon_free(henry);
    ck_assert(!result);
    ck_assert(!dataOut);
    checkLongerListUnchanged();
}
END_TEST

START_TEST(test_list_contains_is_contained) {
    l = _create_longer_list();
    struct pokemon *Squirtle = pokemon_new("Squirtle", "Water", 13);
    struct pokemon *dataOut = (struct pokemon *)0xDEADBEEFU;
    int result = contains(l, (struct pokemon *) Squirtle, &dataOut);
    pokemon_free(Squirtle);
    ck_assert(result);
    ck_assert(dataOut != Squirtle);
    ck_assert(pokemon_name_is(dataOut, "Squirtle"));
    checkLongerListUnchanged();
}
END_TEST

/*************************** zip() tests **************************************/
/******************************************************************************/

START_TEST(test_zip_nonempty_lists) {
    struct linked_list *l1 = _create_longer_list2();
    struct linked_list *l2 = _create_longer_list2();
    l2->head->data->name[3] = 0;
    l2->head->next->data->name[3] = 0;
    l2->head->next->next->data->name[3] = 0;
    zip(l1, l2);
    ck_assert(l1);
    ck_assert_int_eq(l1->size, 6);
    ck_assert(pokemon_name_is(l1->head->data, "Pikachu"));
    ck_assert(pokemon_name_is(l1->head->next->data, "Pik"));
    ck_assert(pokemon_name_is(l1->head->next->next->data, "Squirtle"));
    ck_assert(pokemon_name_is(l1->head->next->next->next->data, "Squ"));
    ck_assert(pokemon_name_is(l1->head->next->next->next->next->data, "Charmander"));
    ck_assert(pokemon_name_is(l1->head->next->next->next->next->next->data, "Cha"));
    ck_assert(!l2->head);
    ck_assert_int_eq(l2->size, 0);
    _free_list_nodes_and_data(l1, pokemon_free);
    _free_list_nodes_and_data(l2, pokemon_free);
}
END_TEST

START_TEST(test_zip_first_list_empty) {
    struct linked_list *l1 = _create_list();
    struct linked_list *l2 = _create_longer_list2();
    zip(l1, l2);
    ck_assert(l1);
    ck_assert_int_eq(l1->size, 3);
    ck_assert(pokemon_name_is(l1->head->data, "Pikachu"));
    ck_assert(pokemon_name_is(l1->head->next->data, "Squirtle"));
    ck_assert(pokemon_name_is(l1->head->next->next->data, "Charmander"));
    ck_assert(!l2->head);
    ck_assert_int_eq(l2->size, 0);
    _free_list_nodes_and_data(l1, pokemon_free);
    _free_list_nodes_and_data(l2, pokemon_free);
}
END_TEST

START_TEST(test_zip_second_list_empty) {
    struct linked_list *l1 = _create_longer_list2();
    struct linked_list *l2 = _create_list();
    zip(l1, l2);
    ck_assert(l1);
    ck_assert_int_eq(l1->size, 3);
    ck_assert(pokemon_name_is(l1->head->data, "Pikachu"));
    ck_assert(pokemon_name_is(l1->head->next->data, "Squirtle"));
    ck_assert(pokemon_name_is(l1->head->next->next->data, "Charmander"));
    ck_assert(!l2->head);
    ck_assert_int_eq(l2->size, 0);
    _free_list_nodes_and_data(l1, pokemon_free);
    _free_list_nodes_and_data(l2, pokemon_free);
}
END_TEST

START_TEST(test_zip_null_list) {
    struct linked_list *l1 = NULL;
    struct linked_list *l2 = _create_longer_list2();
    zip(l1, l2);
    ck_assert(l2);
    ck_assert_int_eq(l2->size, 3);
    ck_assert(pokemon_name_is(l2->head->data, "Pikachu"));
    ck_assert(pokemon_name_is(l2->head->next->data, "Squirtle"));
    ck_assert(pokemon_name_is(l2->head->next->next->data, "Charmander"));
    ck_assert(!l1);
    _free_list_nodes_and_data(l2, pokemon_free);
}
END_TEST

#endif
/******************************************************************************/
/*************************** swap() tests *************************************/
/******************************************************************************/

/* TODO */

START_TEST(test_swap_pokemon_simple) {
    struct linked_list *l1 = _create_longer_list2();
    Node* node1 = _create_node(pokemon_new("Pikachu", "Electric", 2));
    Node* node2 = _create_node(pokemon_new("Charmander", "Fire", 1));

    // printf("SIMPLE\nBefore: ");
    // //print_list(l1, 0);

    swap_pokemon(l1, node1, node2);

    // //printf("After: ");
    // //print_list(l1, 1);

    ck_assert(l1);
    ck_assert_int_eq(l1->size, 3);
    ck_assert(pokemon_name_is(l1->head->data, "Charmander"));
    ck_assert(pokemon_name_is(l1->head->next->data, "Squirtle"));
    ck_assert(pokemon_name_is(l1->head->next->next->data, "Pikachu"));
    ck_assert(!l1->head->next->next->next);

    ck_assert(_cmp_pokemon(node1->data, &((Pokemon){"Pikachu", "Electric", 2})));           // make sure that node1 didnt change
    ck_assert(_cmp_pokemon(node2->data, &((Pokemon){"Charmander", "Fire", 1})));    // make sure that node2 didnt change

    // Free
    _free_node(node1);
    _free_node(node2);
    _free_list_nodes_and_data(l1, pokemon_free);
}
END_TEST

START_TEST(test_swap_pokemon_simple_test_diff_nodes) {
    struct linked_list *l1 = _create_longer_list2();
    Node* node1 = _create_node(pokemon_new("Charmander", "Fire", 1));
    Node* node2 = _create_node(pokemon_new("Pikachu", "Electric", 2));

    //printf("SIMPLE TEST DIFF NODES\nBefore: ");
    //print_list(l1, 0);

    swap_pokemon(l1, node1, node2);

    //printf("After: ");
    //print_list(l1, 1);

    ck_assert(l1);
    ck_assert_int_eq(l1->size, 3);
    ck_assert(pokemon_name_is(l1->head->data, "Charmander"));
    ck_assert(pokemon_name_is(l1->head->next->data, "Squirtle"));
    ck_assert(pokemon_name_is(l1->head->next->next->data, "Pikachu"));
    ck_assert(!l1->head->next->next->next);

    ck_assert(_cmp_pokemon(node1->data, &((Pokemon){"Charmander", "Fire", 1})));    // make sure that node1 didnt change
    ck_assert(_cmp_pokemon(node2->data, &((Pokemon){"Pikachu", "Electric", 2})));           // make sure that node2 didnt change
    

    // Free
    _free_node(node1);
    _free_node(node2);
    _free_list_nodes_and_data(l1, pokemon_free);
}
END_TEST

/* INITIAL: Magikarp->Dratini->Eevee->Pikachu->Squirtle->Charmander */
/* EXPECTED: Magikarp->Dratini->Eevee->Charmander->Squirtle->Pikachu */
START_TEST(test_swap_pokemon_really_long_list){
    struct linked_list *l1 = _create_even_longer_list();
    
    //printf("REALLY LONG LIST\nBefore: ");
    //print_list(l1, 0);
    
    Node* node1 = _create_node(pokemon_new("Charmander", "Fire", 1));
    Node* node2 = _create_node(pokemon_new("Pikachu", "Electric", 5));
    swap_pokemon(l1, node1, node2);

    //printf("After: ");
    //print_list(l1, 1);
    

    ck_assert(l1);
    ck_assert_int_eq(l1->size, 6);
    ck_assert(pokemon_name_is(l1->head->data, "Magikarp"));
    ck_assert(pokemon_name_is(l1->head->next->data, "Dratini"));
    ck_assert(pokemon_name_is(l1->head->next->next->data, "Eevee"));
    ck_assert(pokemon_name_is(l1->head->next->next->next->data, "Charmander"));
    ck_assert(pokemon_name_is(l1->head->next->next->next->next->data, "Squirtle"));
    ck_assert(pokemon_name_is(l1->head->next->next->next->next->next->data, "Pikachu"));
    ck_assert(!l1->head->next->next->next->next->next->next);

    ck_assert(_cmp_pokemon(node1->data, &((Pokemon){"Charmander", "Fire", 1})));    // make sure that node1 didnt change
    ck_assert(_cmp_pokemon(node2->data, &((Pokemon){"Pikachu", "Electric", 5})));           // make sure that node2 didnt change

    // Free
    _free_node(node1);
    _free_node(node2);
    _free_list_nodes_and_data(l1, pokemon_free);

}
END_TEST

/* INITIAL: Magikarp->Dratini->Eevee->Pikachu->Squirtle->Charmander */
/* EXPECTED: Magikarp->Squirtle->Eevee->Pikachu->Dratini->Charmander */
START_TEST(test_swap_pokemon_really_long_list_take_2){
    struct linked_list *l1 = _create_even_longer_list();

    //printf("REALLY LONG LIST 2\nBefore: ");
    //print_list(l1, 0);
    
    Node* node1 = _create_node(pokemon_new("Squirtle", "Water", 4));
    Node* node2 = _create_node(pokemon_new("Dratini", "Dragon", 8));
    swap_pokemon(l1, node1, node2);

    //printf("After: ");
    //print_list(l1, 1);

    ck_assert(l1);
    ck_assert_int_eq(l1->size, 6);
    ck_assert(pokemon_name_is(l1->head->data, "Magikarp"));
    ck_assert(pokemon_name_is(l1->head->next->data, "Squirtle"));
    ck_assert(pokemon_name_is(l1->head->next->next->data, "Eevee"));
    ck_assert(pokemon_name_is(l1->head->next->next->next->data, "Pikachu"));
    ck_assert(pokemon_name_is(l1->head->next->next->next->next->data, "Dratini"));
    ck_assert(pokemon_name_is(l1->head->next->next->next->next->next->data, "Charmander"));
    ck_assert(!l1->head->next->next->next->next->next->next);

    ck_assert(_cmp_pokemon(node1->data, &((Pokemon){"Squirtle", "Water", 4})));  // make sure that node1 didnt change
    ck_assert(_cmp_pokemon(node2->data, &((Pokemon){"Dratini", "Dragon", 8})));  // make sure that node2 didnt change

    // Free
    _free_node(node1);
    _free_node(node2);
    _free_list_nodes_and_data(l1, pokemon_free);

}
END_TEST

START_TEST(test_swap_pokemon_list_is_NULL) {
    struct linked_list *l1 = NULL;
    Node* node1 = _create_node(pokemon_new("Charmander", "Fire", 1));
    Node* node2 = _create_node(pokemon_new("Pikachu", "Electric", 2));

    swap_pokemon(NULL, node1, node2);

    ck_assert(!l1);
    ck_assert(_cmp_pokemon(node1->data, &((Pokemon){"Charmander", "Fire", 1})));    // make sure that node1 didnt change
    ck_assert(_cmp_pokemon(node2->data, &((Pokemon){"Pikachu", "Electric", 2})));           // make sure that node2 didnt change

    // Free
    _free_node(node1);
    _free_node(node2);
}
END_TEST

START_TEST(test_swap_pokemon_node1_is_NULL) {
    struct linked_list *l1 = _create_longer_list2();
    Node* node1 = NULL;
    Node* node2 = _create_node(pokemon_new("Pikachu", "Electric", 2));

    swap_pokemon(l1, node1, node2);

    ck_assert(l1);
    ck_assert(!node1);
    ck_assert(_cmp_pokemon(node2->data, &((Pokemon){"Pikachu", "Electric", 2}))); // make sure that node2 didnt change

    // free
    _free_node(node2);
    _free_list_nodes_and_data(l1, pokemon_free);
}
END_TEST

START_TEST(test_swap_pokemon_node2_is_NULL) {
    struct linked_list *l1 = _create_longer_list2();
    Node* node1 = _create_node(pokemon_new("Pikachu", "Electric", 2));
    Node* node2 = NULL;

    swap_pokemon(l1, node1, node2);

    ck_assert(l1);
    ck_assert(_cmp_pokemon(node1->data, &((Pokemon){"Pikachu", "Electric", 2}))); // make sure that node2 didnt change
    ck_assert(!node2);

    // free
    _free_node(node1);
    _free_list_nodes_and_data(l1, pokemon_free);
}
END_TEST

START_TEST(test_swap_pokemon_everything_is_NULL) {
    struct linked_list *l1 = NULL;
    Node* node1 = NULL;
    Node* node2 = NULL;

    swap_pokemon(l1, node1, node2);

    ck_assert(!l1);
    ck_assert(!node1);
    ck_assert(!node2);
}
END_TEST

START_TEST(test_swap_pokemon_list_size_1) {
    struct linked_list *l1 = _create_list_size_1();
    Node* node1 = _create_node(pokemon_new("Pikachu", "Electric", 2));
    Node* node2 = _create_node(pokemon_new("Charmander", "Fire", 1));

    //printf("list size 1\nBefore: ");
    //print_list(l1, 0);

    swap_pokemon(l1, node1, node2);

    //printf("After: ");
    //print_list(l1, 1);

    ck_assert(l1);
    ck_assert_int_eq(l1->size, 1);
    ck_assert(pokemon_name_is(l1->head->data, "Charmander"));
    ck_assert(!l1->head->next);

    ck_assert(_cmp_pokemon(node1->data, &((Pokemon){"Pikachu", "Electric", 2})));           // make sure that node1 didnt change
    ck_assert(_cmp_pokemon(node2->data, &((Pokemon){"Charmander", "Fire", 1})));    // make sure that node2 didnt change

    // Free
    _free_node(node1);
    _free_node(node2);
    _free_list_nodes_and_data(l1, pokemon_free);
}
END_TEST

START_TEST(test_swap_pokemon_list_size_2) {
    struct linked_list *l1 = _create_list_size_2();
    Node* node1 = _create_node(pokemon_new("Pikachu", "Electric", 2));
    Node* node2 = _create_node(pokemon_new("Charmander", "Fire", 1));

    //printf("list size 2\nBefore: ");
    //print_list(l1, 0);

    swap_pokemon(l1, node1, node2);

    //printf("After: ");
    //print_list(l1, 1);

    ck_assert(l1);
    ck_assert_int_eq(l1->size, 2);
    ck_assert(pokemon_name_is(l1->head->data, "Charmander"));
    ck_assert(pokemon_name_is(l1->head->next->data, "Pikachu"));
    ck_assert(!l1->head->next->next);

    ck_assert(_cmp_pokemon(node1->data, &((Pokemon){"Pikachu", "Electric", 2})));           // make sure that node1 didnt change
    ck_assert(_cmp_pokemon(node2->data, &((Pokemon){"Charmander", "Fire", 1})));    // make sure that node2 didnt change

    // Free
    _free_node(node1);
    _free_node(node2);
    _free_list_nodes_and_data(l1, pokemon_free);
}
END_TEST

START_TEST(test_swap_pokemon_list_does_not_contain_node1) {
    struct linked_list *l1 = _create_longer_list2();
    Node* node1 = _create_node(pokemon_new("Dratini", "Fire", 1));
    Node* node2 = _create_node(pokemon_new("Pikachu", "Electric", 2));

    //printf("list size 2\nBefore: ");
    //print_list(l1, 0);

    swap_pokemon(l1, node1, node2);

    //printf("After: ");
    //print_list(l1, 1);

    ck_assert(l1);
    ck_assert_int_eq(l1->size, 3);
    ck_assert(pokemon_name_is(l1->head->data, "Pikachu"));
    ck_assert(pokemon_name_is(l1->head->next->data, "Squirtle"));
    ck_assert(pokemon_name_is(l1->head->next->next->data, "Charmander"));
    ck_assert(!l1->head->next->next->next);

    ck_assert(_cmp_pokemon(node1->data, &((Pokemon){"Dratini", "Fire", 1})));   // make sure that node1 didnt change
    ck_assert(_cmp_pokemon(node2->data, &((Pokemon){"Pikachu", "Electric", 2})));           // make sure that node2 didnt change

    // Free
    _free_node(node1);
    _free_node(node2);
    _free_list_nodes_and_data(l1, pokemon_free);
}
END_TEST

START_TEST(test_swap_pokemon_list_does_not_contain_node2) {
    struct linked_list *l1 = _create_longer_list2();
    Node* node1 = _create_node(pokemon_new("Pikachu", "Electric", 2));
    Node* node2 = _create_node(pokemon_new("Bulbasaur", "Water", 1));

    //printf("list size 2\nBefore: ");
    //print_list(l1, 0);

    swap_pokemon(l1, node1, node2);

    //printf("After: ");
    //print_list(l1, 1);

    ck_assert(l1);
    ck_assert_int_eq(l1->size, 3);
    ck_assert(pokemon_name_is(l1->head->data, "Pikachu"));
    ck_assert(pokemon_name_is(l1->head->next->data, "Squirtle"));
    ck_assert(pokemon_name_is(l1->head->next->next->data, "Charmander"));
    ck_assert(!l1->head->next->next->next);

    ck_assert(_cmp_pokemon(node1->data, &((Pokemon){"Pikachu", "Electric", 2})));           // make sure that node1 didnt change
    ck_assert(_cmp_pokemon(node2->data, &((Pokemon){"Bulbasaur", "Water", 1})));    // make sure that node2 didnt change

    // Free
    _free_node(node1);
    _free_node(node2);
    _free_list_nodes_and_data(l1, pokemon_free);
}
END_TEST

START_TEST(test_swap_pokemon_list_does_not_contain_either_node) {
    struct linked_list *l1 = _create_longer_list2();
    Node* node1 = _create_node(pokemon_new("Eevee", "Normal", 2));
    Node* node2 = _create_node(pokemon_new("Bulbasaur", "Water", 1));

    swap_pokemon(l1, node1, node2);

    ck_assert(l1);
    ck_assert_int_eq(l1->size, 3);
    ck_assert(pokemon_name_is(l1->head->data, "Pikachu"));
    ck_assert(pokemon_name_is(l1->head->next->data, "Squirtle"));
    ck_assert(pokemon_name_is(l1->head->next->next->data, "Charmander"));
    ck_assert(!l1->head->next->next->next);

    ck_assert(_cmp_pokemon(node1->data, &((Pokemon){"Eevee", "Normal", 2})));           // make sure that node1 didnt change
    ck_assert(_cmp_pokemon(node2->data, &((Pokemon){"Bulbasaur", "Water", 1})));    // make sure that node2 didnt change

    // Free
    _free_node(node1);
    _free_node(node2);
    _free_list_nodes_and_data(l1, pokemon_free);
}
END_TEST



// List that does not contain node1, node2, either
Suite *list_suite(void) {
    (void)_create_even_longer_list;
    Suite *s = suite_create("struct linked_list");
    
    #if TESTING_SWAP_TESTS
    // create_list() tests
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_create_list_basic);
    tcase_hack(s, reset_mallocs_until_fail, NULL, test_list_create_list_malloc_failure);

    // push_front() tests
    tcase_hack(s, setup_list_push_front, teardown_list_push_front, test_list_push_front_NULL_list);
    tcase_hack(s, setup_list_push_front, teardown_list_push_front, test_list_push_front_NULL_data);
    tcase_hack(s, setup_list_push_front, teardown_list_push_front, test_list_push_front_empty);
    tcase_hack(s, setup_list_push_front, teardown_list_push_front, test_list_push_front_nonempty);

    // push_back()tests
    tcase_hack(s, setup_list_push_front, teardown_list_push_front, test_list_push_back_empty);
    tcase_hack(s, setup_list_push_front, teardown_list_push_front, test_list_push_back_non_empty);
    tcase_hack(s, setup_list_push_front, teardown_list_push_front, test_list_push_back_null);

    // add_at_index() tests
    tcase_hack(s, setup_list_push_front, teardown_list_push_front, test_list_add_at_index_out_of_bounds);
    tcase_hack(s, setup_list_push_front, teardown_list_push_front, test_list_add_at_index_empty_list);
    tcase_hack(s, setup_list_push_front, teardown_list_push_front, test_list_add_at_index_non_empty_list);
    tcase_hack(s, setup_list_push_front, teardown_list_push_front, test_list_add_at_index_null);

    // pop_front() tests
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_pop_front_NULL_list);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_pop_front_NULL_dataOut);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_pop_front_empty);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_pop_front_nonempty);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_pop_front_longer_list);

    // pop_back() tests
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_pop_back_NULL_list);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_pop_back_NULL_dataOut);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_pop_back_empty);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_pop_back_nonempty);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_pop_back_longer_list);

    // remove_at_index() tests
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_remove_at_index_NULL_list);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_remove_at_index_NULL_dataOut);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_remove_at_index_out_of_bounds);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_remove_at_index_front);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_remove_at_index_middle);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_remove_at_index_back);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_remove_at_index_length_1);

    // empty_list() tests
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_empty_list_NULL_list);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_empty_list_empty);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_empty_list_length_1);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_empty_list_longer_list);

    // get() tests
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_get_NULL_list);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_get_NULL_dataOut);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_get_negative_index);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_get_front);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_get_middle);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_get_back);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_get_out_of_bounds_index);

    // contains() tests
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_contains_NULL_list);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_contains_NULL_dataOut);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_contains_empty);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_contains_not_contained);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_list_contains_is_contained);

    // zip() tests
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_zip_nonempty_lists);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_zip_first_list_empty);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_zip_second_list_empty);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_zip_null_list);
    
    #endif

    // swap_pokemon() tests
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_swap_pokemon_simple);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_swap_pokemon_simple_test_diff_nodes);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_swap_pokemon_really_long_list);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_swap_pokemon_really_long_list_take_2);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_swap_pokemon_list_is_NULL);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_swap_pokemon_node1_is_NULL);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_swap_pokemon_node2_is_NULL);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_swap_pokemon_everything_is_NULL);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_swap_pokemon_list_size_1);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_swap_pokemon_list_size_2);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_swap_pokemon_list_does_not_contain_node1);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_swap_pokemon_list_does_not_contain_node2);
    tcase_hack(s, reset_mallocs_until_fail, teardown_bar_list, test_swap_pokemon_list_does_not_contain_either_node);

    return s;
}
