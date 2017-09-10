#pragma once

#define cast(type, value) *(type*)&value

#define iterator(name)         __iterator_ ## name
#define iterator_make(name)    __iterator_make_ ## name
#define iterator_next(name)    __iterator_next_ ## name
#define iterator_get(name)     __iterator_get_ ## name
#define iterator_get_ptr(name) __iterator_get_ptr_ ## name

#define foreach_it(name, item_name, iterator_name, collection)                                                         \
for (iterator(name)(collection) iterator_name = iterator_make(name)(collection); iterator_next(name)(iterator_name);)  \
for (typeof(iterator_get(name)(iterator_name)) item_name = iterator_get(name)(iterator_name) ;; ({ continue; }))

#define foreach(name, item_name, collection) foreach_it(name, item_name, it, collection)

#define foreach_ptr_it(name, ptr_name, iterator_name, collection)                                                      \
for (iterator(name)(collection) iterator_name = iterator_make(name)(collection); iterator_next(name)(iterator_name);)  \
for (typeof(iterator_get_ptr(name)(iterator_name)) ptr_name = iterator_get_ptr(name)(iterator_name) ;; ({ continue; }))

#define foreach_ptr(name, ptr_name, collection) foreach_ptr_it(name, ptr_name, it, collection)