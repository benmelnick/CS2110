#ifndef BAR_H
#define BAR_H

extern int copies_until_malloc_fail;
extern int bars_visited;

typedef struct pokemon {
    char *name;
    char *type;
    int level;
} Pokemon;

Pokemon *pokemon_new(char *name, char *type, int level);
void pokemon_free(Pokemon *p);
int pokemon_copy(const Pokemon *p, Pokemon **ret);
int pokemon_eq(const Pokemon *a, const Pokemon *b);
int pokemon_compare(const Pokemon *a, const Pokemon *b);
int pokemon_eq_name(const Pokemon *a, const Pokemon *b);
int pokemon_hash(const Pokemon *a);

#endif
