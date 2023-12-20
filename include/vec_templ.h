#ifndef VecT
# define VecT VecOfVoid
# error "'VecT' for the template is not defined!"
#endif

#ifndef T
# define T void *
# error "'T' for the template is not defined!"
#endif

#include <stddef.h>

#include "vec.h"

#define private_vec_fn(name) private_vec_fn1(VecT, name)

#define this_vec_init_vtbl private_vec_fn(init_vtbl)

#define this_vec_init          private_vec_fn(init)
#define this_vec_init_from_arr private_vec_fn(init_from_arr)
#define this_vec_init_filled   private_vec_fn(init_filled)

#define this_vec_at    private_vec_fn(at)
#define this_vec_first private_vec_fn(first)
#define this_vec_last  private_vec_fn(last)

#define this_vec_push   private_vec_fn(push)
#define this_vec_insert private_vec_fn(insert)
#define this_vec_remove private_vec_fn(remove)
#define this_vec_pop    private_vec_fn(pop)

#define this_vec_clear private_vec_fn(clear)

private_decl_vec(VecT, T);

static Result this_vec_init(VecT *const self) {
  return any_vec_init((AnyVec *const)self, sizeof(*self->data));
}

static T *this_vec_at(const VecT *const self, const size_t index) {
  return index >= self->len ? NULL : self->data + index;
}
static T *this_vec_first(const VecT *const self) {
  return self->len == 0 ? NULL : self->data;
}
static T *this_vec_last(const VecT *const self) {
  return self->len == 0 ? NULL : self->data + self->len - 1;
}

static Result this_vec_push(VecT *const self, const T element) {
  unroll(any_vec_increment((AnyVec *const)self));
  *vec_last(self) = element;
  return Ok;
}
static Result
this_vec_insert(VecT *const self, const size_t index, const T element) {
  if (index >= self->len) return RangeErr;

  unroll(any_vec_increment((AnyVec *const)self));

  for (T *e = self->data + self->len - 1; e >= self->data + index + 1; e--)
    *e = *(e - 1);

  self->data[index] = element;

  return Ok;
}
static Result this_vec_remove(VecT *const self, const size_t index) {
  if (index >= self->len) return RangeErr;

  for (T *e = self->data + index; e < self->data + self->len - 1; e++)
    *e = *(e + 1);
  return any_vec_decrement((AnyVec *const)self);
}
static Result this_vec_pop(VecT *const self) {
  // TODO return the last element
  return any_vec_decrement((AnyVec *const)self);
}

// init extensions

static Result this_vec_init_from_arr(
  VecT *const self,
  const T *const arr,
  const size_t arr_len
) {
  unroll(any_vec_init((AnyVec *const)self, sizeof(*self->data)));
  for (const T *el = arr; el < arr + arr_len; el++)
    unroll(this_vec_push(self, *el));
  return Ok;
}
static Result
this_vec_init_filled(VecT *const self, const T element, const size_t n) {
  unroll(any_vec_init((AnyVec *const)self, sizeof(*self->data)));
  for (size_t i = 0; i < n; i++) unroll(this_vec_push(self, element));
  return Ok;
}

// vtbl

void this_vec_init_vtbl(VecT *const self) {
  static const private_vec_vtbl(VecT) vtbl = {
    .init = this_vec_init,
    .init_from_arr = this_vec_init_from_arr,
    .init_filled = this_vec_init_filled,
    .uninit = (void (*)(VecT *const))any_vec_uninit,

    .at = this_vec_at,
    .first = this_vec_first,
    .last = this_vec_last,

    .push = this_vec_push,
    .insert = this_vec_insert,
    .remove = this_vec_remove,
    .pop = this_vec_pop,

    .clear = (Result(*)(VecT *const))any_vec_reset,
  };
  self->vtbl = &vtbl;
}

#undef this_vec_init_vtbl
#undef this_vec_init
#undef this_vec_init_from_arr
#undef this_vec_init_filled
#undef this_vec_at
#undef this_vec_first
#undef this_vec_last
#undef this_vec_push
#undef this_vec_insert
#undef this_vec_remove
#undef this_vec_pop

#undef VecT
#undef T
