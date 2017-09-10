#pragma once

#include <stdint.h>

#define array_t(type) struct { int64_t capacity; type* data; }
#define array_ptr_at(array, index) ({ assert(0 <= index && index < array.capacity); &array.data[index]; })
#define array_at(array, index) *array_ptr_at(array, index)

#define array_slice_from_length(array, from, length) ({                                 \
    typeof(from) __from = from;                                                         \
    typeof(length) __length = length;                                                   \
    assert(__from + __length <= (array).capacity && 0 <= __from);                       \
    (typeof(array)) { .capacity = __length, .data = (array).data + __from };            \
})
#define array_slice_length(array, length) array_slice_from_length(array, 0, length)
#define array_slice_from_to(array, from, to) array_slice_from_length(array, from, (to) - (from))

#define __iterator_array(array) struct { int64_t index; typeof((array).data) start, end; }
#define __iterator_make_array(array) { .index = -1, .start = (array).data, .end = (array).data + (array).capacity }
#define __iterator_next_array(it) (++((it).index) + (it).start) != (it).end
#define __iterator_get_ptr_array(it) ((it).index + (it).start)
#define __iterator_get_array(it) *__iterator_get_ptr_array(it)