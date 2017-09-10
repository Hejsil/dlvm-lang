#pragma once

#include <stdbool.h>
#include "arraylist.h"

typedef struct map_s {
    bool (*equals)(void*, void*);
    uint64_t (*to_hash)(void*);
    arraylist_t hashs;
    arraylist_t keys;
    arraylist_t values;
} map_t;

map_t map_make();
void map_init(map_t* map);
