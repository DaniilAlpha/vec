#include "any_vec.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "restrict.h"

/*************
 ** private **
 *************/

static __always_inline uint8_t *
any_vec_at_nocheck(AnyVec const *const RESTRICT self, size_t const index) {
    return ((uint8_t *)self->_data) + self->_el_size * index;
}

static void any_vec_preinit(AnyVec *const self, size_t const el_size) {
    self->_len = self->_cap = 0;
    self->_el_size = el_size;
    self->_data = NULL;
}

static Result any_vec_resize(AnyVec *const RESTRICT self, size_t new_cap) {
    if (new_cap < VEC_MIN_CAP) new_cap = VEC_MIN_CAP;
    if (self->_cap == new_cap) return Ok;

    uint8_t *const new_data = realloc(self->_data, self->_el_size * new_cap);
    if (new_data == NULL) return OutOfMemErr;

    self->_cap = new_cap;
    self->_data = new_data;

    return Ok;
}

static inline Result any_vec_increment(AnyVec *const self) {
    if (self->_len == self->_cap) UNROLL(any_vec_resize(self, self->_cap << 1));
    self->_len++;
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
    self->_len = self->_cap = self->_el_size = 0;
    free(self->_data), self->_data = NULL;
}

Result any_vec_init_from_arr(
    AnyVec *const self,
    size_t const el_size,
    uint8_t const *const RESTRICT arr,
    size_t const len
) {
    any_vec_preinit(self, el_size);
    UNROLL(any_vec_resize(self, el_size * len));

    memcpy(self->_data, arr, el_size * len);

    self->_len = len;

    return Ok;
}
Result any_vec_init_filled(
    AnyVec *const self,
    size_t const el_size,
    uint8_t const *const RESTRICT element,
    size_t const n
) {
    any_vec_preinit(self, el_size);
    UNROLL(any_vec_resize(self, el_size * n));

    // WARN possible bug due to restrict usage
    for (uint8_t *RESTRICT el = (uint8_t *)self->_data;
         el <= any_vec_at_nocheck(self, n - 1);
         el += el_size)
        memcpy(el, element, el_size);

    self->_len = n;

    return Ok;
}

uint8_t *any_vec_at(AnyVec const *const RESTRICT self, size_t const index) {
    if (index >= self->_len) return NULL;
    uint8_t *const ptr = any_vec_at_nocheck(self, index);
    if (ptr < any_vec_at_nocheck(self, 0)) return NULL;  // TODO maybe useless
    return ptr;
}

Result any_vec_push(AnyVec *const self, uint8_t const *const RESTRICT element) {
    UNROLL(any_vec_increment(self));
    memcpy(any_vec_at_nocheck(self, self->_len - 1), element, self->_el_size);
    return Ok;
}
Result any_vec_insert(
    AnyVec *const self,
    size_t const index,
    uint8_t const *const RESTRICT element
) {
    if (index > self->_len)
        return RangeErr;
    else if (index == self->_len)
        return any_vec_push(self, element);

    UNROLL(any_vec_increment(self));

    uint8_t *const pos = any_vec_at_nocheck(self, index);
    memmove(
        pos + self->_el_size,
        pos,
        self->_el_size * ((self->_len - 1) - index)
    );
    memcpy(pos, element, self->_el_size);

    return Ok;
}
Result any_vec_remove(AnyVec *const self, size_t const index) {
    if (index >= self->_len) return RangeErr;
    if (index < self->_len - 1) {
        uint8_t *const pos = any_vec_at_nocheck(self, index);
        memmove(
            pos,
            pos + self->_el_size,
            self->_el_size * ((self->_len - 1) - index)
        );
    }
    return any_vec_decrement(self);
}

Result any_vec_decrement(AnyVec *const self) {
    if (self->_len == 0) return RangeErr;

    size_t const new_cap = self->_cap >> 1;
    if (new_cap >= self->_len - 1) UNROLL(any_vec_resize(self, new_cap));

    self->_len--;

    return Ok;
}

Result any_vec_reset(AnyVec *const self) {
    size_t const el_size = self->_el_size;
    return any_vec_uninit(self), any_vec_init(self, el_size);
}
