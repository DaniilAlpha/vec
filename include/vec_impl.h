#ifndef VecT
# define VecT VecOfPtr
# error "'VecT' for the template is not defined!"
#endif

#ifndef ElT
# define ElT void const *
# error "'ElT' for the template is not defined!"
#endif

#include <stddef.h>

#include "any_vec.h"

#define private_vec_fn(name) private_vec_fn1(VecT, name)

#define this_vec_init          private_vec_fn(init)
#define this_vec_init_from_arr private_vec_fn(init_from_arr)
#define this_vec_init_filled   private_vec_fn(init_filled)

#define this_vec_push   private_vec_fn(push)
#define this_vec_insert private_vec_fn(insert)

#define this_vec_init_vtbl private_vec_fn(init_vtbl)

typedef struct VecT VecT;

static Result this_vec_init(VecT *const self) {
  return any_vec_init((AnyVec *)self, sizeof(*self->data));
}

static Result this_vec_init_from_arr(
  VecT *const self,
  ElT const *const arr,
  size_t const len
) {
  return any_vec_init_from_arr((AnyVec *)self, sizeof(*self->data), arr, len);
}
static Result
this_vec_init_filled(VecT *const self, ElT const element, size_t const n) {
  return any_vec_init_filled((AnyVec *)self, sizeof(*self->data), &element, n);
}

static Result this_vec_push(VecT *const self, ElT const element) {
  return any_vec_push((AnyVec *)self, &element);
}
static Result
this_vec_insert(VecT *const self, size_t const index, ElT const element) {
  return any_vec_insert((AnyVec *)self, index, &element);
}

void this_vec_init_vtbl(VecT *const self) {
  static private_vec_vtbl(VecT) const vtbl = {
    .init = this_vec_init,
    .init_from_arr = this_vec_init_from_arr,
    .init_filled = this_vec_init_filled,
    .uninit = (void (*)(VecT *const))any_vec_uninit,

    .at = (ElT * (*)(VecT const *const, size_t const)) any_vec_at,
    .first = (ElT * (*)(VecT const *const)) any_vec_first,
    .last = (ElT * (*)(VecT const *const)) any_vec_last,

    .push = this_vec_push,
    .insert = this_vec_insert,
    .remove = (Result(*)(VecT *const, size_t const))any_vec_remove,
    .pop = (Result(*)(VecT *const))any_vec_decrement,

    .clear = (Result(*)(VecT *const))any_vec_reset,
  };
  self->vtbl = &vtbl;
}

#undef this_vec_init
#undef this_vec_init_from_arr
#undef this_vec_init_filled
#undef this_vec_push
#undef this_vec_insert
#undef this_vec_init_vtbl

#undef private_vec_fn

#undef VecT
#undef ElT
