#ifndef VEC_H
#define VEC_H

#include <stddef.h>

#include <result.h>

#define VEC_MIN_CAP (1)

#define private_vec_vtbl1(VecT) VecT##Vtbl
#define private_vec_vtbl(VecT)  private_vec_vtbl1(VecT)

#define private_vec_fn2(type, name) (private_##type##_##name)
#define private_vec_fn1(type, name) private_vec_fn2(type, name)

#define private_decl_vec(VecT, ElT)                                            \
    typedef struct private_vec_vtbl(VecT) private_vec_vtbl(VecT);              \
    typedef struct VecT {                                                      \
        size_t len, cap;                                                       \
        size_t el_size;                                                        \
        ElT *data;                                                             \
        private_vec_vtbl(VecT) const *vtbl;                                    \
    } VecT;                                                                    \
    struct private_vec_vtbl(VecT) {                                            \
        Result (*const init)(VecT *const);                                     \
        Result (*const init_from_arr)(                                         \
            VecT *const,                                                       \
            ElT const *const,                                                  \
            size_t const                                                       \
        );                                                                     \
        Result (*const init_filled)(VecT *const, ElT const, size_t const);     \
        void (*const uninit)(VecT *const);                                     \
                                                                               \
        ElT *(*const at)(VecT const *const, size_t const);                     \
        ElT *(*const first)(VecT const *const);                                \
        ElT *(*const last)(VecT const *const);                                 \
                                                                               \
        Result (*const push)(VecT *const, ElT const);                          \
        Result (*const insert)(VecT *const, size_t const, ElT const);          \
        Result (*const remove)(VecT *const, size_t const);                     \
        Result (*const pop)(VecT *const);                                      \
                                                                               \
        Result (*const clear)(VecT *const);                                    \
    }

#define private_vec_vtbl_init(self, VecT) private_vec_fn1(VecT, init_vtbl)(self)

/// @brief Inits an ampty vec.
/// @param self valid pointer
/// @param VecT vector type (for getting vtable)
/// @return `Result`
#define vec_init(self, VecT)                                                   \
    (private_vec_vtbl_init((self), VecT), (self)->vtbl->init(self))

/// @brief Inits a vec and populates it with values from `arr`.
/// @param self valid pointer
/// @param VecT vector type (for getting vtable)
/// @param arr array to take values from
/// @param len length of `arr`
/// @return `Result`
#define vec_init_from_arr(self, VecT, arr, len)                                \
    (private_vec_vtbl_init((self), VecT),                                      \
     (self)->vtbl->init_from_arr((self), (arr), (len)))

/// @brief Inits a vec and populates it `n` times with `element` value from
/// `arr`.
/// @param self valid pointer
/// @param VecT vector type (for getting vtable)
/// @param element value to populate with
/// @param n how many times `element` is repeated
/// @return `Result`
#define vec_init_filled(self, VecT, element, n)                                \
    (private_vec_vtbl_init((self), VecT),                                      \
     (self)->vtbl->init_filled((self), (element), (n)))
/// @brief Uninits a vec. Using other functions on an uninited vec can cause
/// undefined behavior.
/// @param self valid pointer
/// @return `void`
#define vec_uninit(self) ((self)->vtbl->uninit(self))

/// @brief Get vec's len.
/// @param self valid pointer
/// @return vec's len
#define vec_len(self) ((size_t const)(self)->len)
/// @brief Get vec's capacity.
/// @param self valid pointer
/// @return vec's capacity
#define vec_cap(self) ((size_t const)(self)->cap)
/// @brief Get vec's internal buffer.
/// @param self valid pointer
/// @return pointer to vec's buffer
#define vec_buf(self) ((void const *const)(self)->data)

/// @brief Gets the element (with bounds checking).
/// @param self valid pointer
/// @param index any number
/// @return
///   element pointer - success;
///   `NULL` - bounds check failed
#define vec_at(self, index) ((self)->vtbl->at((self), (index)))

/// @brief Gets the first element.
/// @param self valid pointer
/// @return
///   element pointer - success;
///   `NULL` – vec is empty
#define vec_first(self) ((self)->vtbl->first(self))

/// @brief Gets the last element.
/// @param self valid pointer
/// @return
///   element pointer - success;
///   `NULL` – vec is empty
#define vec_last(self) ((self)->vtbl->last(self))

/// @brief Pushes an element into a vec.
/// @param self valid pointer
/// @param element value to push
/// @return `Result`
#define vec_push(self, element) ((self)->vtbl->push((self), (element)))

/// @brief Inserts an element into a vec.
/// @param self valid pointer
/// @param index index at which to insert
/// @param element value to insert
/// @return `Result`
#define vec_insert(self, index, element)                                       \
    ((self)->vtbl->insert((self), (index), (element)))

/// @brief Removes an element from a vec.
/// @param self valid pointer
/// @param index index at which to remove
/// @return `Result`
#define vec_remove(self, index) ((self)->vtbl->remove((self), (index)))

/// @brief Pops an element from a vec.
/// @param self valid pointer
/// @return `Result`
#define vec_pop(self) ((self)->vtbl->pop(self))

/// @brief Clears a vec.
/// @param self valid pointer
/// @return `Result` (technically, failure is impossable, but who really knows?)
#define vec_clear(self) ((self)->vtbl->clear(self))

typedef void const *Any;
private_decl_vec(AnyVec, Any);

Result any_vec_init(AnyVec *const self, size_t const el_size);
void any_vec_uninit(AnyVec *const self);

Result any_vec_init_from_arr(
    AnyVec *const self,
    size_t const el_size,
    Any const *const arr,
    size_t const len
);
Result any_vec_init_filled(
    AnyVec *const self,
    size_t const el_size,
    Any const *const element,
    size_t const n
);

void *any_vec_at(AnyVec const *const self, size_t const index);
void *any_vec_first(AnyVec const *const self);
void *any_vec_last(AnyVec const *const self);

Result any_vec_push(AnyVec *const self, Any const *const element);
Result any_vec_insert(
    AnyVec *const self,
    size_t const index,
    Any const *const element
);
Result any_vec_remove(AnyVec *const self, size_t const index);

Result any_vec_decrement(AnyVec *const self);

Result any_vec_reset(AnyVec *const self);

#endif
