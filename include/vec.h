#ifndef VEC_H
#define VEC_H

#include <stdbool.h>
#include <stddef.h>

#define VEC_MIN_CAP (1)

#define decl_vec(name, T)                                                      \
  typedef struct name {                                                        \
    size_t len, cap;                                                           \
    T *data;                                                                   \
    size_t el_size;                                                            \
  } name;

decl_vec(AnyVec, void);

bool any_vec_init(AnyVec *const self, const size_t el_size);
void any_vec_uninit(AnyVec *const self);

bool any_vec_increment(AnyVec *const self);
bool any_vec_decrement(AnyVec *const self);

bool any_vec_reset(AnyVec *const self);

#endif
