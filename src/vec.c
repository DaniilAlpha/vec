#include "vec.h"

#include <stdlib.h>

Result any_vec_init(AnyVec *const self, const size_t el_size) {
  const size_t new_cap = VEC_MIN_CAP;

  self->data = malloc(el_size * new_cap);
  if (self->data == NULL) return OutOfMemErr;

  self->len = 0;
  self->cap = new_cap;
  self->el_size = el_size;

  return Ok;
}

void any_vec_uninit(AnyVec *const self) {
  self->len = self->cap = self->el_size = 0;
  free(self->data), self->data = NULL;
}

Result any_vec_increment(AnyVec *const self) {
  if (self->len == self->cap) {
    const size_t new_cap = self->cap << 1;
    if (new_cap == 0) return RangeErr;

    void *const new_data = realloc(self->data, self->el_size * new_cap);
    if (new_data == NULL) return OutOfMemErr;

    self->cap = new_cap;
    self->data = new_data;
  }

  self->len++;

  return Ok;
}

Result any_vec_decrement(AnyVec *const self) {
  if (self->len == 0) return RangeErr;

  const size_t new_cap = self->cap >> 1;
  if (new_cap >= self->len - 1 && new_cap >= VEC_MIN_CAP) {
    void *const new_data = realloc(self->data, self->el_size * new_cap);
    if (new_data == NULL) return OutOfMemErr;

    self->cap = new_cap;
    self->data = new_data;
  }

  self->len--;

  return Ok;
}

Result any_vec_reset(AnyVec *const self) {
  const size_t el_size = self->el_size;
  any_vec_uninit(self);
  return any_vec_init(self, el_size);
}
