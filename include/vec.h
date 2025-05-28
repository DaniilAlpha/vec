#ifndef Self
#  define Self VecOfPtr
#  error "'Self' for the template is not defined!"
#endif

#ifndef T
#  define T void const *
#  error "'T' for the template is not defined!"
#endif

#ifndef PREFIX
#  define PREFIX Self
#endif

#include "any_vec.h"

INTERNAL__VEC_DECL(Self, T);

#define INTERNAL__METHOD1(PREFIX, NAME) (PREFIX##_##NAME)
#define INTERNAL__METHOD(PREFIX, NAME)  INTERNAL__METHOD1(PREFIX, NAME)
#define INTERNAL__VEC_METHOD(NAME)      INTERNAL__METHOD(PREFIX, NAME)

#define this_vec_init          INTERNAL__VEC_METHOD(init)
#define this_vec_init_from_arr INTERNAL__VEC_METHOD(init_from_arr)
#define this_vec_init_filled   INTERNAL__VEC_METHOD(init_filled)
#define this_vec_uninit        INTERNAL__VEC_METHOD(uninit)

#define this_vec_len INTERNAL__VEC_METHOD(get_len)
#define this_vec_cap INTERNAL__VEC_METHOD(get_cap)
#define this_vec_buf INTERNAL__VEC_METHOD(get_buf)

#define this_vec_at    INTERNAL__VEC_METHOD(at)
#define this_vec_start INTERNAL__VEC_METHOD(start)
#define this_vec_end   INTERNAL__VEC_METHOD(end)

#define this_vec_push   INTERNAL__VEC_METHOD(push)
#define this_vec_insert INTERNAL__VEC_METHOD(insert)
#define this_vec_remove INTERNAL__VEC_METHOD(remove)
#define this_vec_pop    INTERNAL__VEC_METHOD(pop)

#define this_vec_clear INTERNAL__VEC_METHOD(clear)

/// Inits an ampty vec.
/// `self` - valid pointer.
static inline Result this_vec_init(Self *const self) {
    self->_el_size = sizeof(self->_data[0]);
    return any_vec_init((AnyVec *)self);
}
/// Inits a vec and populates it with values from `arr`.
/// `self` - valid pointer.
/// `arr` - array to take values from.
/// `len` - length of `arr`.
static inline Result
this_vec_init_from_arr(Self *const self, T const *const arr, size_t const len) {
    return any_vec_init_from_arr((AnyVec *)self, (uint8_t const *)arr, len);
}
/// Inits a vec and populates it `n` times with the `element` value.
/// `self` - valid pointer.
/// `element` - value to populate with.
/// `n` - how many times `element` is repeated.
static inline Result
this_vec_init_filled(Self *const self, T const element, size_t const n) {
    return any_vec_init_filled((AnyVec *)self, (uint8_t const *)&element, n);
}
/// Uninits a vec. Using other functions on an uninited vec can cause undefined
/// behavior.
/// `self` - valid pointer.
static inline void this_vec_uninit(Self *const self) {
    self->_el_size = 0;
    any_vec_uninit((AnyVec *)self);
}

/// Get vec's len.
/// `self` - valid pointer.
static inline size_t this_vec_len(Self const *const self) { return self->_len; }
/// Get vec's capacity.
/// `self` - valid pointer.
static inline size_t this_vec_cap(Self const *const self) { return self->_cap; }
/// Get vec's internal buffer.
/// `self` - valid pointer.
static inline void *this_vec_buf(Self const *const self) { return self->_data; }

/// Gets the element, or `NULL` if `index` is out of
/// bounds.
/// `self` - valid pointer.
/// `index` - any number.
static inline T *this_vec_at(Self const *const self, size_t const index) {
    return (T *)any_vec_at((AnyVec const *)self, index);
}
/// Gets the vec's start (the first element), or `NULL` if vec is empty.
/// `self` - valid pointer.
static inline T *this_vec_start(Self const *const self) {
    return self->_len == 0 ? NULL : self->_data;
}
/// Gets the vec's end (just past the last element), or `NULL` if vec is empty.
/// `self` - valid pointer.
static inline T *this_vec_end(Self const *const self) {
    return self->_len == 0 ? NULL : self->_data + self->_len;
}

/// Pushes an element into a vec.
/// `self` - valid pointer.
/// `element` - value to push.
static inline Result this_vec_push(Self *const self, T const element) {
    return any_vec_push((AnyVec *)self, (uint8_t const *)&element);
}
/// Inserts an element into a vec.
/// `self` - valid pointer.
/// `index` - index at which to insert.
/// `element` - value to insert.
static inline Result
this_vec_insert(Self *const self, size_t const index, T const element) {
    return any_vec_insert((AnyVec *)self, index, (uint8_t const *)&element);
}
/// Removes an element from a vec.
/// `self` - valid pointer.
/// `index` - index at which to remove.
static inline Result this_vec_remove(Self *const self, size_t const index) {
    return any_vec_remove((AnyVec *)self, index);
}
/// Pops an element from a vec.
/// `self` - valid pointer.
static inline Result this_vec_pop(Self *const self) {
    return any_vec_decrement((AnyVec *)self);
}

/// Clears a vec. Technically can fail, but very unlikely.
/// `self` - valid pointer.
static inline Result this_vec_clear(Self *const self) {
    return any_vec_reset((AnyVec *)self);
}

#undef this_vec_init
#undef this_vec_init_from_arr
#undef this_vec_init_filled
#undef this_vec_uninit

#undef this_vec_len
#undef this_vec_cap
#undef this_vec_buf

#undef this_vec_at
#undef this_vec_start
#undef this_vec_end

#undef this_vec_push
#undef this_vec_insert
#undef this_vec_remove
#undef this_vec_pop

#undef this_vec_clear

#undef INTERNAL__METHOD1
#undef INTERNAL__METHOD
#undef INTERNAL__VEC_METHOD

#undef PREFIX
#undef Self
#undef T
