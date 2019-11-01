#include <check.h>
#include <stdlib.h>
#include "pokemon.h"

int copies_until_malloc_fail = -1;
int bars_visited = 0;

/**
 * Hashes a pokemon based on the first letter in their name. Names that start
 * with upper case letters hash to negative numbers, and those with lower case
 * letters will hash to 0 - 26.
 */
int pokemon_hash(const Pokemon *a) {
    Pokemon *pokemon_a = (Pokemon *)a;
    return (pokemon_a->name)[0] - 97;
}

/**
 * Allocates a new pokemon struct on the heap with the given name and word.
 */
Pokemon * pokemon_new(char *name, char *type, int level) {
    Pokemon *result = malloc(sizeof (Pokemon));
    ck_assert(result);

    result->name = malloc(strlen(name) + 1);
    ck_assert(result->name);
    strcpy(result->name, name);

    result->type = malloc(strlen(type) + 1);
    ck_assert(result->type);
    strcpy(result->type, type);

    result->level = level;

    return result;
}

/**
 * Deallocates the memory for a pokemon struct.
 */
void pokemon_free(Pokemon *p) {
    // At the moment, I don't add NULL pointers
    ck_assert(p);

    Pokemon *b = (Pokemon *) p;
    free(b->name);
    free(b->type);
    free(b);

    bars_visited++;
}

// Used in test_dll_null_data.
// This is separate from pokemon_free() because I want to catch stray NULL data
// pointers outside test_dll_null_data
void pokemon_free_null(Pokemon *p) {
    ck_assert(!p);

    bars_visited++;
}

/**
 * Copies the data for a pokemon struct to newly allocated heap memory.
 */
int pokemon_copy(const Pokemon *p, Pokemon **ret) {
    if (ret == NULL){
        return 0;
    }

    struct pokemon *b = (struct pokemon *) p;
    *ret = pokemon_new(b->name, b->type, b->level);
    return 1;
}

// Used in test_dll_null_data.
// This is separate from pokemon_copy() because I want to catch stray NULL data
// pointers outside test_dll_null_data
Pokemon *pokemon_copy_null(const Pokemon *p) {
    ck_assert(!p);

    bars_visited++;

    return NULL;
}

// Used in test_dll_malloc_fail.
// To prevent valgrind from reporting leaks in copy_dllist() allowed by the
// TAs, return the pointer passed in.
Pokemon *pokemon_copy_fake(const Pokemon *p) {
    if (!copies_until_malloc_fail) {
        // Simulate a failed malloc in copy_func()
        return NULL;
    } else {
        if (copies_until_malloc_fail != -1) {
            copies_until_malloc_fail--;
        }

        // TODO: This cast is a hack, but it shouldn't cause any issues because
        //       we never write to it. It's just a non-NULL value to send back
        return (struct pokemon *) p;
    }
}

void pokemon_visit(Pokemon *p) {
    // At the moment, I don't add NULL pointers
    ck_assert(p);
    bars_visited++;
}

// Used in test_dll_null_data.
// This is separate from pokemon_visit() because I want to catch stray NULL data
// pointers outside test_dll_null_data
void pokemon_visit_null(Pokemon *p) {
    ck_assert(!p);
    bars_visited++;
}

/**
 * @brief Compare two pokemon structs
 * 
 * @param poke1 
 * @param poke2 
 * @return int 1 on success and 0 on failure
 */
int pokemon_eq(const Pokemon* poke1, const Pokemon* poke2){
    if(!poke1 || !poke2) return 0;
    if(!strcmp(poke1->name, poke2->name) && !strcmp(poke1->type, poke2->type) && (poke1->level == poke2->level)) return 1;
    return 0;
}

/**
 * Returns the string comparison of the two pokemon structs' names. Check out
 * the man page for strcmp! (Fun fact, if you're on vim you can put your cursor
 * over the word strcmp (or any c standard library function) and press
 * shift+k to automatically open the man page)
 */
int pokemon_eq_name(const Pokemon *a, const Pokemon *b) {
    ck_assert(a);
    ck_assert(b);
    struct pokemon *pokemon_a = (struct pokemon *) a;
    struct pokemon *pokemon_b = (struct pokemon *) b;

    bars_visited++;

    return strcmp(pokemon_a->name, pokemon_b->name);
}

/**
 * Returns the string comparison of the two pokemon structs' levels. 
 */
int pokemon_compare(const Pokemon *a, const Pokemon *b) {
    ck_assert(a);
    ck_assert(b);

    return a->level - b->level;
}

// Used in test_dll_null_data.
// This is separate from pokemon_eq() because I want to catch stray NULL data
// pointers outside test_dll_null_data
// Always find the first match
int pokemon_eq_null_match(const Pokemon *a, const Pokemon *b) {
    ck_assert(!a);
    ck_assert(!b);

    bars_visited++;

    return 0;
}

// Same, but never finds a match
int pokemon_eq_null_no_match(const Pokemon *a, const Pokemon *b) {
    ck_assert(!a);
    ck_assert(!b);

    bars_visited++;

    return 1;
}
