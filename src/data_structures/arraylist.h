#pragma once

#include <inttypes.h>

typedef struct {
    int8_t* data;
    int64_t count, capacity, element_size;
} arraylist_t;

arraylist_t arraylist_make(int64_t element_size, int64_t capacity);
void arraylist_init(arraylist_t *array, int64_t element_size, int64_t capacity);
void arraylist_deinit(arraylist_t *array);
void* arraylist_unsafe_get_ptr(arraylist_t *array, int64_t index);
void* arraylist_allocate_at(arraylist_t *array, int64_t index);
void* arraylist_allocate_next(arraylist_t *array);
void arraylist_add(arraylist_t *array, int64_t index, void *element);
void arraylist_add_last(arraylist_t *array, void *element);
void arraylist_remove(arraylist_t *array, int64_t index);
void arraylist_remove_last(arraylist_t *array);
void arraylist_get(arraylist_t *array, int64_t index, void *result);
void* arraylist_get_ptr(arraylist_t *array, int64_t index);
void arraylist_get_last(arraylist_t *array, void *result);
void* arraylist_get_ptr_last(arraylist_t *array);