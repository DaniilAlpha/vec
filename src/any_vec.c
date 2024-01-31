#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-arith"

#include "any_vec.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*************
 ** private **
 *************/

static Result any_vec_resize(AnyVec *const self, size_t new_cap) {
  if (new_cap < VEC_MIN_CAP) {
    if (self->cap < new_cap) {
      new_cap = VEC_MIN_CAP;
    } else {
      return Ok;
    }
  }

  void *const new_data = realloc(self->data, self->el_size * new_cap);
  if (new_data == NULL) return OutOfMemErr;

  self->cap = new_cap;
  self->data = new_data;

  return Ok;
}

static Result any_vec_increment(AnyVec *const self) {
  if (self->len == self->cap) unroll(any_vec_resize(self, self->cap << 1));

  self->len++;

  return Ok;
}

/************
 ** public **
 ************/

Result any_vec_init(AnyVec *const self, size_t const el_size) {
  self->len = 0;
  self->cap = 0;
  self->data = NULL;
  self->el_size = el_size;

  any_vec_resize(self, VEC_MIN_CAP);

  return Ok;
}
void any_vec_uninit(AnyVec *const self) {
  self->len = self->cap = self->el_size = 0;
  free(self->data), self->data = NULL;
}

Result any_vec_init_from_arr(
  AnyVec *const self,
  size_t const el_size,
  void const *const arr,
  size_t const len
) {
  self->len = 0;
  self->cap = 0;
  self->data = NULL;
  self->el_size = el_size;

  unroll(any_vec_resize(self, el_size * len));
  memcpy(self->data, arr, el_size * len);

  self->len = len;

  return Ok;
}
Result any_vec_init_filled(
  AnyVec *const self,
  size_t const el_size,
  void const *const element,
  size_t const n
) {
  self->len = 0;
  self->cap = 0;
  self->data = NULL;
  self->el_size = el_size;

  unroll(any_vec_resize(self, el_size * n));
  for (void *el = self->data; el <= ((void *)self->data + el_size * (n - 1));
       el += el_size)
    memcpy(el, element, el_size);

  self->len = n;

  return Ok;
}

void *any_vec_at(AnyVec const *const self, size_t const index) {
  if (index >= self->len) return NULL;
  void *const ptr = ((void *)self->data) + self->el_size * index;
  if (ptr < ((void *)self->data)) return NULL;
  return ptr;
}
void *any_vec_first(AnyVec const *const self) {
  return self->len == 0 ? NULL : self->data;
}
void *any_vec_last(AnyVec const *const self) {
  return self->len == 0
         ? NULL
         : ((void *)self->data) + self->el_size * (self->len - 1);
}

Result any_vec_push(AnyVec *const self, void const *const element) {
  unroll(any_vec_increment(self));
  memcpy(any_vec_last(self), element, self->el_size);
  return Ok;
}
Result any_vec_insert(
  AnyVec *const self,
  size_t const index,
  void const *const element
) {
  if (index > self->len) return RangeErr;
  if (index == self->len) return any_vec_push(self, element);

  unroll(any_vec_increment(self));

  memmove(
    any_vec_at(self, index + 1),
    any_vec_at(self, index),
    self->el_size * ((self->len - 1) - index)
  );
  memcpy(any_vec_at(self, index), element, self->el_size);

  return Ok;
}
Result any_vec_remove(AnyVec *const self, size_t const index) {
  if (index >= self->len) return RangeErr;

  memmove(
    any_vec_at(self, index),
    any_vec_at(self, index + 1),
    self->el_size * ((self->len - 1) - (index + 1))
  );

  return any_vec_decrement(self);
}

Result any_vec_decrement(AnyVec *const self) {
  if (self->len == 0) return RangeErr;

  size_t const new_cap = self->cap >> 1;
  if (new_cap >= self->len - 1) unroll(any_vec_resize(self, new_cap));

  self->len--;

  return Ok;
}

Result any_vec_reset(AnyVec *const self) {
  size_t const el_size = self->el_size;
  return any_vec_uninit(self), any_vec_init(self, el_size);
}

#pragma GCC diagnostic pop
