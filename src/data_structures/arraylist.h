#pragma once

#include <inttypes.h>
#include <assert.h>
#include <stdbool.h>


typedef struct {
    void* data;
    int64_t count, capacity, item_size;
} arraylist_t;


#define DEFAULT_ARRAYLIST_CAP 2
void arraylist_unsafe_init(arraylist_t *array, int64_t item_size, int64_t capacity);
#define arraylist_init_cap(array, type, capacity)         \
typedef type __type__; /* Checks if type is a type */     \
arraylist_unsafe_init(array, sizeof(type), capacity);
#define arraylist_init(array, type) arraylist_init_cap(array, type, DEFAULT_ARRAYLIST_CAP)


arraylist_t arraylist_unsafe_make(int64_t item_size, int64_t capacity);
#define arraylist_make_cap(type, capacity) ({             \
    arraylist_t __result;                                 \
    arraylist_init_cap(&__result, type, capacity);        \
    __result;                                             \
})
#define arraylist_make(type) arraylist_make_cap(type, DEFAULT_ARRAYLIST_CAP)


void arraylist_remove(arraylist_t *array, int64_t index);
void arraylist_remove_last(arraylist_t *array);


void* arraylist_unsafe_allocate_at(arraylist_t *array, int64_t index);
void* arraylist_unsafe_allocate_next(arraylist_t *array);
void* arraylist_unsafe_get_void_ptr(arraylist_t *array, int64_t index);
void* arraylist_get_void_ptr(arraylist_t *array, int64_t index);

// NOTE: We can't have an arraylist where each element is an fixed size array with this
#define arraylist_get_ptr(array, type, index) ({          \
    typedef type __type__; /* Checks if type is a type */ \
    assert(sizeof(type) == (array)->item_size);        \
    (type*)arraylist_get_void_ptr(array, index);          \
})

#define arraylist_get_ptr_last(array, type) arraylist_get_ptr(array, type, (array)->count - 1)


#define arraylist_get(array, type, index) *arraylist_get_ptr(array, type, index)
#define arraylist_get_last(array, type) *arraylist_get_ptr(array, type, (array)->count)


void arraylist_unsafe_add(arraylist_t *array, int64_t index, void *element);
#define arraylist_verbose_add(array, index, item, variable_name) do { \
    assert(sizeof(typeof(item)) == (array)->item_size);               \
    typeof(item) variable_name = (item);                              \
    arraylist_unsafe_add(array, index, &(variable_name));             \
} while(0)
#define arraylist_add(array, index, item) \
arraylist_verbose_add(array, index, item, __temp_variable__ ## __COUNTER__)
#define arraylist_add_last(array, item) \
arraylist_add(array, (array)->count, item)


void arraylist_deinit(arraylist_t *array);
void arraylist_resize(arraylist_t* array, int64_t new_size);


void arraylist_unsafe_set(arraylist_t* array, int64_t index, void* item);
#define arraylist_set(array, type, index, item) do {    \
    assert(sizeof(type) == (array)->item_size);         \
    type __temp_item__ = item;                          \
    arraylist_unsafe_set(array, index, &__temp_item__); \
} while(0)
