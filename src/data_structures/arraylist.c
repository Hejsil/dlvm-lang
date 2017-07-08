#include <mem.h>
#include <assert.h>
#include <stdlib.h>
#include "arraylist.h"

arraylist_t arraylist_make(int64_t element_size, int64_t capacity) {
    arraylist_t result;
    arraylist_init(&result, element_size, capacity);

    return result;
}

void arraylist_init(arraylist_t *array, int64_t element_size, int64_t capacity) {
    assert(capacity > 0);
    assert(element_size > 0);

    array->count = 0;
    array->element_size = element_size;
    array->capacity = capacity;
    array->data = NULL;
}

void arraylist_deinit(arraylist_t *array) {
    assert(array->capacity > 0);
    assert(array->element_size > 0);

    array->count = 0;
    array->element_size = 0;
    array->capacity = 0;

    if (array->data != NULL) {
        free(array->data);
        array->data = NULL;
    }
}

void* arraylist_allocate_at(arraylist_t *array, int64_t index) {
    assert(index >= 0);
    assert(array->element_size > 0);
    assert(array->capacity > 0);
    assert(array->count >= index);
    assert(array->count <= array->capacity);

    if (array->data == NULL) {
        array->data = malloc((size_t) (array->element_size * array->capacity));
    } else if (array->capacity == array->count) {
        array->capacity *= 2;
        array->data = realloc(array->data, (size_t) (array->capacity * array->element_size));
    }

    memmove(arraylist_unsafe_get_ptr(array, index + 1),
            arraylist_unsafe_get_ptr(array, index),
            (size_t) ((array->count - index) * array->element_size));
    array->count++;

    return arraylist_unsafe_get_ptr(array, index);
}

void* arraylist_allocate_next(arraylist_t *array) {
    return arraylist_allocate_at(array, array->count);
}

void arraylist_add(arraylist_t *array, int64_t index, void *element) {
    arraylist_allocate_at(array, index);
    memcpy(arraylist_unsafe_get_ptr(array, index), element, (size_t) array->element_size);
}

void arraylist_add_last(arraylist_t *array, void *element) {
    arraylist_add(array, array->count, element);
}

void arraylist_remove(arraylist_t *array, int64_t index) {
    assert(index >= 0);
    assert(array->element_size > 0);
    assert(array->count > 0);
    assert(index < array->count);
    assert(array->data != NULL);

    array->count--;
    memmove(arraylist_unsafe_get_ptr(array, index),
            arraylist_unsafe_get_ptr(array, index + 1),
            (size_t) ((array->count - index) * array->element_size));

#if !NDEBUG
    memset(arraylist_unsafe_get_ptr(array, array->count), 0, (size_t) (array->capacity - array->count));
#endif
}

void arraylist_remove_last(arraylist_t *array) {
    arraylist_remove(array, array->count - 1);
}

void arraylist_get(arraylist_t *array, int64_t index, void *result) {
    assert(array->element_size > 0);
    assert(index >= 0);
    assert(index < array->count);
    assert(array->data != NULL);

    memcpy(result, arraylist_unsafe_get_ptr(array, index), (size_t) array->element_size);
}

void* arraylist_get_ptr(arraylist_t *array, int64_t index) {
    assert(array->element_size > 0);
    assert(index >= 0);
    assert(index < array->count);
    assert(array->data != NULL);

    return arraylist_unsafe_get_ptr(array, index);
}

void arraylist_get_last(arraylist_t *array, void *result) {
    arraylist_get(array, array->count - 1, result);
}

void* arraylist_get_ptr_last(arraylist_t *array) {
    return arraylist_get_ptr(array, array->count - 1);
}

void* arraylist_unsafe_get_ptr(arraylist_t *array, int64_t index) {
    return &array->data[index * array->element_size];
}