#ifndef Self
#  define Self VecOfPtr
#  error "'Self' for the template is not defined!"
#endif

#ifndef T
#  define T Any
#  error "'T' for the template is not defined!"
#endif

#include <stddef.h>

#include <result.h>

#include "any_vec.h"
#include "vec_decl.h"

#define private_vec_method(name) private_method(Self, name)

#define this_vec_init          private_vec_method(init)
#define this_vec_init_from_arr private_vec_method(init_from_arr)
#define this_vec_init_filled   private_vec_method(init_filled)

#define this_vec_push   private_vec_method(push)
#define this_vec_insert private_vec_method(insert)

#define this_vec_init_vtbl private_vec_method(init_vtbl)

typedef struct Self Self;

static Result this_vec_init(Self *const self) {
    return any_vec_init((AnyVec *)self, sizeof(*self->data));
}

static Result
this_vec_init_from_arr(Self *const self, T const *const arr, size_t const len) {
    return any_vec_init_from_arr(
        (AnyVec *)self,
        sizeof(*self->data),
        (Any const *)arr,
        len
    );
}
static Result
this_vec_init_filled(Self *const self, T const element, size_t const n) {
    return any_vec_init_filled(
        (AnyVec *)self,
        sizeof(*self->data),
        (Any const *)&element,
        n
    );
}

static Result this_vec_push(Self *const self, T const element) {
    return any_vec_push((AnyVec *)self, (Any const *)&element);
}
static Result
this_vec_insert(Self *const self, size_t const index, T const element) {
    return any_vec_insert((AnyVec *)self, index, (Any const *)&element);
}

void this_vec_init_vtbl(Self *const self) {
    static private_vec_vtbl(Self) const vtbl = {
        .init = this_vec_init,
        .init_from_arr = this_vec_init_from_arr,
        .init_filled = this_vec_init_filled,
        .uninit = (void (*)(Self *const))any_vec_uninit,

        .at = (T * (*)(Self const *const, size_t const)) any_vec_at,

        .push = this_vec_push,
        .insert = this_vec_insert,
        .remove = (Result(*)(Self *const, size_t const))any_vec_remove,
        .pop = (Result(*)(Self *const))any_vec_decrement,

        .clear = (Result(*)(Self *const))any_vec_reset,
    };
    self->vtbl = &vtbl;
}

#undef this_vec_init
#undef this_vec_init_from_arr
#undef this_vec_init_filled
#undef this_vec_push
#undef this_vec_insert
#undef this_vec_init_vtbl

#undef private_vec_method

#undef Self
#undef T
