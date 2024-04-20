#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-arith"

#include "any_vec.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "restrict.h"

/*************
 ** private **
 *************/

static __always_inline void *
any_vec_at_nocheck(AnyVec const *const restrict self, size_t const index) {
    return ((void *)self->data) + self->el_size * index;
}

static void any_vec_preinit(AnyVec *const self, size_t const el_size) {
    self->len = self->cap = 0;
    self->el_size = el_size;
    self->data = NULL;
}

static Result any_vec_resize(AnyVec *const restrict self, size_t new_cap) {
    if (new_cap < VEC_MIN_CAP) {
        // TODO questinable, but for some reason was here
        // if (self->cap >= new_cap) return Ok;
        new_cap = VEC_MIN_CAP;
    }

    void *const new_data = realloc(self->data, self->el_size * new_cap);
    if (new_data == NULL) return OutOfMemErr;

    self->cap = new_cap;
    self->data = new_data;

    return Ok;
}

static inline Result any_vec_increment(AnyVec *const self) {
    if (self->len == self->cap) unroll(any_vec_resize(self, self->cap << 1));
    self->len++;
    return Ok;
}

/************
 ** public **
 ************/

Result any_vec_init(AnyVec *const self, size_t const el_size) {
    any_vec_preinit(self, el_size);
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
    void const *const restrict arr,
    size_t const len
) {
    any_vec_preinit(self, el_size);
    unroll(any_vec_resize(self, el_size * len));

    memcpy(self->data, arr, el_size * len);

    self->len = len;

    return Ok;
}
Result any_vec_init_filled(
    AnyVec *const self,
    size_t const el_size,
    void const *const restrict element,
    size_t const n
) {
    any_vec_preinit(self, el_size);
    unroll(any_vec_resize(self, el_size * n));

    // WARN possible bug due to restrict usage
    for (void *restrict el = self->data; el <= any_vec_at_nocheck(self, n - 1);
         el += el_size)
        memcpy(el, element, el_size);

    self->len = n;

    return Ok;
}

void *any_vec_at(AnyVec const *const restrict self, size_t const index) {
    if (index >= self->len) return NULL;
    void *const ptr = any_vec_at_nocheck(self, index);
    if (ptr < any_vec_at_nocheck(self, 0)) return NULL;  // TODO maybe useless
    return ptr;
}

Result any_vec_push(AnyVec *const self, void const *const restrict element) {
    unroll(any_vec_increment(self));
    memcpy(any_vec_at_nocheck(self, self->len - 1), element, self->el_size);
    return Ok;
}
Result any_vec_insert(
    AnyVec *const self,
    size_t const index,
    void const *const restrict element
) {
    if (index > self->len)
        return RangeErr;
    else if (index == self->len)
        return any_vec_push(self, element);

    unroll(any_vec_increment(self));

    void *const pos = any_vec_at_nocheck(self, index);
    memmove(
        pos + self->el_size,
        pos,
        self->el_size * ((self->len - 1) - index)
    );
    memcpy(pos, element, self->el_size);

    return Ok;
}
Result any_vec_remove(AnyVec *const self, size_t const index) {
    if (index >= self->len) return RangeErr;
    if (index < self->len - 1) {
        void *const pos = any_vec_at_nocheck(self, index);
        memmove(
            pos,
            pos + self->el_size,
            self->el_size * ((self->len - 1) - index)
        );
    }
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
