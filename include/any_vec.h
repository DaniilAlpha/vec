#ifndef ANY_VEC_H
#define ANY_VEC_H

#include <stddef.h>
#include <stdint.h>

#include <result.h>

#define VEC_MIN_CAP (1)

#define INTERNAL__VEC_DECL(Self, T)                                            \
    typedef struct Self {                                                      \
        T *_data;                                                              \
        size_t _len, _cap;                                                     \
        size_t _el_size;                                                       \
    } Self

INTERNAL__VEC_DECL(AnyVec, uint8_t);

Result any_vec_init(AnyVec *const self);
void any_vec_uninit(AnyVec *const self);

Result any_vec_init_from_arr(
    AnyVec *const self,
    uint8_t const *const arr,
    size_t const len
);
Result any_vec_init_filled(
    AnyVec *const self,
    uint8_t const *const element,
    size_t const n
);

uint8_t *any_vec_at(AnyVec const *const self, size_t const index);

Result any_vec_push(AnyVec *const self, uint8_t const *const element);
Result any_vec_insert(
    AnyVec *const self,
    size_t const index,
    uint8_t const *const element
);
Result any_vec_remove(AnyVec *const self, size_t const index);

Result any_vec_decrement(AnyVec *const self);

Result any_vec_reset(AnyVec *const self);

#endif
