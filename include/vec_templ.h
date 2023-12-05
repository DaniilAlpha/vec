#ifndef VecT
# define VecT VecOfVoid
# error "'VecT' for the template is not defined!"
#endif

#ifndef T
# define T void
# error "'T' for the template is not defined!"
#endif

#ifndef VEC_FN_PREFIX
# define VEC_FN_PREFIX vec
# error "'VEC_FN_PREFIX' for the template is not defined!"
#endif

#include <stddef.h>

#include "vec.h"

#define vec_fn2(prefix, name) (prefix##_##name)
#define vec_fn1(prefix, name) vec_fn2(prefix, name)
#define vec_fn(name)          vec_fn1(VEC_FN_PREFIX, name)

/// @brief Inits an ampty vec.
/// @param self valid pointer
/// @return
///   `true` - success;
///   `false` - `malloc` failed
#define vec_init vec_fn(init)

/// @brief Inits a vec and populates it with values from `arr`.
/// @param self valid pointer
/// @param arr array to take values from
/// @param arr_len length of `arr`
/// @return
///   `true` - success;
///   `false` - `malloc` failed
#define vec_init_from_arr vec_fn(init_from_arr)

/// @brief Inits a vec and populates it `n` times with `element` value from
/// `arr`.
/// @param self valid pointer
/// @param element value to populate with
/// @param n how many times `element` repeated
/// @return
///   `true` - success;
///   `false` - `malloc` failed
#define vec_init_filled vec_fn(init_filled)

/// @brief Uninits a vec. Using other functions on an uninited vec can cause
/// undefined behavior.
/// @param self valid pointer
/// @return `void`
#define vec_uninit vec_fn(uninit)

/// @brief Get vec's len.
/// @param self valid pointer
/// @return vec's len
#define vec_len vec_fn(len)

/// @brief Get vec's capacity.
/// @param self valid pointer
/// @return vec's capacity
#define vec_cap vec_fn(cap)

/// @brief Get vec's internal buffer.
/// @param self valid pointer
/// @return pointer to vec's buffer
#define vec_buf vec_fn(buf)

/// @brief Gets the element (with bounds checking).
/// @param self valid pointer
/// @param index any number
/// @return
///   element pointer - success;
///   `NULL` - bounds check failed
#define vec_at vec_fn(at)

/// @brief Gets the first element.
/// @param self valid pointer
/// @return
///   element pointer - success;
///   `NULL` – vec is empty
#define vec_first vec_fn(first)

/// @brief Gets the last element.
/// @param self valid pointer
/// @return
///   element pointer - success;
///   `NULL` – vec is empty
#define vec_last vec_fn(last)

/// @brief Pushes an element into a vec.
/// @param self valid pointer
/// @param element value to push
/// @return
///   `true` - success;
///   `false` - `realloc` failed
#define vec_push vec_fn(push)

/// @brief Inserts an element into a vec.
/// @param self valid pointer
/// @param index any number
/// @param element value to insert
/// @return
///   `true` - success;
///   `false` - either bounds check or `realloc` failed
#define vec_insert vec_fn(insert)

/// @brief Removes an element from a vec.
/// @param self valid pointer
/// @param index any number
/// @return
///   `true` - success;
///   `false` - either vec is empty or `realloc` failed
#define vec_remove vec_fn(remove)

/// @brief Pops an element from a vec.
/// @param self valid pointer
/// @return
///   `true` - success;
///   `false` - either vec is empty or `realloc` failed
#define vec_pop vec_fn(pop)

/// @brief Clears a vec.
/// @param self valid pointer
/// @return
///   `true` - success;
///   `false` - `malloc` failed (technically impossable, but who really knows?)
#define vec_clear vec_fn(clear)

#define forceinline __attribute__((always_inline)) inline

decl_vec(VecT, T);

forceinline bool vec_init(VecT *const self) {
  return any_vec_init((AnyVec *const)self, sizeof(*self->data));
}
forceinline void vec_uninit(VecT *const self) {
  any_vec_uninit((AnyVec *const)self);
}

forceinline size_t vec_len(const VecT *const self) { return self->len; }
forceinline size_t vec_cap(const VecT *const self) { return self->cap; }
forceinline T *vec_buf(const VecT *const self) { return self->data; }

forceinline T *vec_at(const VecT *const self, const size_t index) {
  return index >= self->len ? NULL : self->data + index;
}
forceinline T *vec_first(const VecT *const self) {
  return self->len == 0 ? NULL : self->data;
}
forceinline T *vec_last(const VecT *const self) {
  return self->len == 0 ? NULL : self->data + self->len - 1;
}

forceinline bool vec_push(VecT *const self, const T element) {
  const bool r = any_vec_increment((AnyVec *const)self);
  *vec_last(self) = element;
  return r;
};
forceinline bool
vec_insert(VecT *const self, const size_t index, const T element) {
  if (index >= self->len) return false;

  const bool r = any_vec_increment((AnyVec *const)self);
  for (T *e = self->data + self->len - 1; e >= self->data + index + 1; e--)
    *e = *(e - 1);
  self->data[index] = element;
  return r;
};

forceinline bool vec_remove(VecT *const self, const size_t index) {
  if (index >= self->len) return false;

  for (T *e = self->data + index; e < self->data + self->len - 1; e++)
    *e = *(e + 1);
  return any_vec_decrement((AnyVec *const)self);
};
forceinline bool vec_pop(VecT *const self) {
  // TODO return the last element
  return any_vec_decrement((AnyVec *const)self);
}

forceinline bool vec_clear(VecT *const self) {
  // TODO? maybe not to refurn boolean
  return any_vec_reset((AnyVec *const)self);
}

// init extensions

forceinline bool
vec_init_from_arr(VecT *const self, const T *const arr, const size_t arr_len) {
  const bool r = any_vec_init((AnyVec *const)self, sizeof(*self->data));
  for (const T *el = arr; el < arr + arr_len; el++) vec_push(self, *el);
  return r;
}
forceinline bool
vec_init_filled(VecT *const self, const T element, const size_t n) {
  const bool r = any_vec_init((AnyVec *const)self, sizeof(*self->data));
  for (size_t i = 0; i < n; i++) vec_push(self, element);
  return r;
}

#undef forceinline
#undef vec_fn2
#undef vec_fn1
#undef vec_fn

#undef vec_init
#undef vec_init_from_arr
#undef vec_init_filled
#undef vec_uninit
#undef vec_len
#undef vec_cap
#undef vec_buf
#undef vec_at
#undef vec_first
#undef vec_last
#undef vec_push
#undef vec_insert
#undef vec_remove
#undef vec_pop
#undef vec_clear

#undef VecT
#undef T
#undef VEC_FN_PREFIX
