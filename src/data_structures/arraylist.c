#include <mem.h>
#include <assert.h>
#include <stdlib.h>
#include "arraylist.h"

void arraylist_unsafe_init(arraylist_t *array, int64_t item_size, int64_t capacity) {
    assert(0 < capacity);
    assert(0 < item_size);

    array->count = 0;
    array->item_size = item_size;
    array->capacity = capacity;
    array->data = NULL;
}

void arraylist_deinit(arraylist_t *array) {
    assert(0 < array->capacity);
    assert(0 < array->item_size);

    array->count = 0;
    array->item_size = 0;
    array->capacity = 0;

    if (array->data != NULL) {
        free(array->data);
        array->data = NULL;
    }
}

void* arraylist_unsafe_allocate_at(arraylist_t *array, int64_t index) {
    assert(0 <= index && index <= array->count);
    assert(array->count <= array->capacity);
    assert(0 < array->item_size);
    assert(0 < array->capacity);

    if (array->capacity == array->count) {
        arraylist_resize(array, array->capacity * 2);
    } else if (array->data == NULL) {
        array->data = malloc((size_t) (array->item_size * array->capacity));
    }

    memmove(arraylist_unsafe_get_void_ptr(array, index + 1),
            arraylist_unsafe_get_void_ptr(array, index),
            (size_t) ((array->count - index) * array->item_size));
    array->count++;

    return arraylist_unsafe_get_void_ptr(array, index);
}

void* arraylist_unsafe_allocate_next(arraylist_t *array) {
    return arraylist_unsafe_allocate_at(array, array->count);
}

void arraylist_unsafe_add(arraylist_t *array, int64_t index, void *item) {
    arraylist_unsafe_allocate_at(array, index);
    memcpy(arraylist_unsafe_get_void_ptr(array, index), item, (size_t) array->item_size);
}

void arraylist_remove(arraylist_t *array, int64_t index) {
    assert(0 <= index && index < array->count);
    assert(array->count <= array->capacity);
    assert(0 < array->item_size);
    assert(0 < array->capacity);
    assert(array->data != NULL);

    array->count--;
    memmove(arraylist_unsafe_get_void_ptr(array, index),
            arraylist_unsafe_get_void_ptr(array, index + 1),
            (size_t) ((array->count - index) * array->item_size));

#if !NDEBUG
    memset(arraylist_unsafe_get_void_ptr(array, array->count), 0, (size_t) (array->capacity - array->count));
#endif
}

void arraylist_remove_last(arraylist_t *array) {
    arraylist_remove(array, array->count - 1);
}

void* arraylist_unsafe_get_void_ptr(arraylist_t *array, int64_t index) {
    return array->data + index * array->item_size;
}

void* arraylist_get_void_ptr(arraylist_t *array, int64_t index) {
    assert(0 <= index && index < array->count);
    assert(array->count <= array->capacity);
    assert(0 < array->item_size);
    assert(0 < array->capacity);
    assert(array->data != NULL);

    return arraylist_unsafe_get_void_ptr(array, index);
}

#undef min
#define min(a, b) ({ typeof(a) __a = (a), __b = (b); __a < __b ? __a : __b; })

void arraylist_resize(arraylist_t* array, int64_t new_size) {
    assert(array->count <= array->capacity);
    assert(0 < array->item_size);
    assert(0 < array->capacity);
    assert(array->data != NULL);

    array->capacity = new_size;
    array->count = min(array->capacity, array->count); // TODO: Should we error, if we resize below count?
    array->data = realloc(array->data, (size_t) (array->capacity * array->item_size));
}

void arraylist_unsafe_set(arraylist_t* array, int64_t index, void* item) {
    assert(0 <= index && index < array->count);
    assert(array->count <= array->capacity);
    assert(0 < array->item_size);
    assert(0 < array->capacity);
    assert(array->data != NULL);

    memcpy(arraylist_unsafe_get_void_ptr(array, index), item, (size_t) array->item_size);
}