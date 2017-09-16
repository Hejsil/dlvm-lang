#pragma once

#include <stdint.h>

#define array_t(type) struct { int64_t capacity; type* data; }
#define array_ptr_at(array, index) ({                         \
    typeof(index) __index__ = index;                          \
    typeof(array) __array__ = array;                          \
    assert(0 <= __index__ && __index__ < __array__.capacity); \
    &__array__.data[__index__];                               \
})
#define array_at(array, index) *array_ptr_at(array, index)

#define array_slice_from_length(array, from, length) { .capacity = length, .data = array.data + from };
#define array_slice_length(array, length) array_slice_from_length(array, 0, length)
#define array_slice_from_to(array, from, to) array_slice_from_length(array, from, (to) - (from))
#define array_slice(array) ({ typeof(array) __array = array; array_slice_from_length(__array, 0, __array.capacity) })

#define __iterator_array(array) struct { int64_t index; typeof((array).data) start, end; }
#define __iterator_make_array(array) { .index = -1, .start = (array).data, .end = (array).data + (array).capacity }
#define __iterator_next_array(it) (++((it).index) + (it).start) != (it).end
#define __iterator_get_ptr_array(it) ((it).index + (it).start)
#define __iterator_get_array(it) *__iterator_get_ptr_array(it)

#define __iterator_rev_array(array) struct { int64_t index; typeof((array).data) start, end; }
#define __iterator_make_rev_array(array) { .index = (array).capacity, .start = ((array).data + (array).capacity) - 1, .end = (array).data - 1 }
#define __iterator_next_rev_array(it) ({ int64_t __count = (it).start - (it).end; ((it).start - (__count - (--(it).index + 1))) != (it).end; })
#define __iterator_get_ptr_rev_array(it) ({ int64_t __count = (it).start - (it).end; ((it).start - (__count - ((it).index + 1))); })
#define __iterator_get_rev_array(it) *__iterator_get_ptr_rev_array(it)