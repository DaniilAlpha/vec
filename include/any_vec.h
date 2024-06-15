#ifndef ANY_VEC_H
#define ANY_VEC_H

#include <stddef.h>

#include <result.h>

#include "vec_decl.h"

#define VEC_MIN_CAP (1)

INTERNAL__VEC_DECL(AnyVec, void const *);

Result any_vec_init(AnyVec *const self, size_t const el_size);
void any_vec_uninit(AnyVec *const self);

Result any_vec_init_from_arr(
    AnyVec *const self,
    size_t const el_size,
    void const *const arr,
    size_t const len
);
Result any_vec_init_filled(
    AnyVec *const self,
    size_t const el_size,
    void const *const element,
    size_t const n
);

void *any_vec_at(AnyVec const *const self, size_t const index);

Result any_vec_push(AnyVec *const self, void const *const element);
Result any_vec_insert(
    AnyVec *const self,
    size_t const index,
    void const *const element
);
Result any_vec_remove(AnyVec *const self, size_t const index);

Result any_vec_decrement(AnyVec *const self);

Result any_vec_reset(AnyVec *const self);

#endif
