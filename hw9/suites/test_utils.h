#include "../list.h"
#include "../pokemon.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define tcase_hack(suite, setup_fixture, teardown_fixture, func) { \
    TCase *tc = tcase_create(STRINGIFY(func)); \
    tcase_add_checked_fixture(tc, setup_fixture, teardown_fixture); \
    tcase_add_test(tc, func); \
    suite_add_tcase(s, tc); \
}

typedef Node node;

/* Given to you for free, However you better know how to do this come time for
 * the final exam!
 * A function pointer type that points to a function that takes in a Pokemon * and
 * returns nothing, call it list_op
 */
typedef void (*list_op)(Pokemon *);

/* A function pointer type that points to a function that takes in a Pokemon * and
 * a Pokemon ** (used to pass back a pointer to the newly allocated data) and returns
 * a boolean indicating success or failure, call it list_copy
 */
typedef int (*list_copy)(const Pokemon *, Pokemon **);

/* A function pointer type that points to a function that takes in two Pokemon * and
 * returns an int, call it list_eq
 */
typedef int (*list_eq)(const Pokemon *, const Pokemon *);

extern int _cmp_pokemon(Pokemon* poke1, Pokemon* poke2);
extern node *_create_node(Pokemon *data);
extern struct linked_list *_create_list(void);
extern struct hash_table *_create_hash_table(void);
extern int _push_front(struct linked_list *listToPrependTo, Pokemon *data);
extern void _empty_list(struct linked_list *listToEmpty, list_op freeFunc);
extern void _free_list_nodes_and_data(struct linked_list *listToEmpty, list_op freeFunc);
extern void _free_list_nodes(struct linked_list *listToEmpty);
extern void _free_node(Node* node);
extern int _hash_table_add(struct hash_table *hash_table_to_add_to,
                     Pokemon *data_to_add);
extern void _free_hash_table_and_data(struct hash_table *table, list_op free_func);
