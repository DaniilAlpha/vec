#ifndef VEC_H
#define VEC_H

#include <stddef.h>

#include "result.h"

#define VEC_MIN_CAP (1)

#define vec_vtbl1(VecT) VecT##Vtbl
#define vec_vtbl(VecT)  vec_vtbl1(VecT)

#define decl_vec(VecT, T)                                                      \
  typedef struct vec_vtbl(VecT) vec_vtbl(VecT);                                \
  typedef struct VecT {                                                        \
    size_t len, cap;                                                           \
    T *data;                                                                   \
    size_t el_size;                                                            \
    const vec_vtbl(VecT) * vtbl;                                               \
  } VecT;                                                                      \
  struct vec_vtbl(VecT) {                                                      \
    Result (*const init)(VecT *const);                                         \
    Result (*const init_from_arr)(VecT *const, const T *const, const size_t);  \
    Result (*const init_filled)(VecT *const, const T, const size_t);           \
    void (*const uninit)(VecT *const);                                         \
                                                                               \
    T *(*const at)(const VecT *const, const size_t);                           \
    T *(*const first)(const VecT *const);                                      \
    T *(*const last)(const VecT *const);                                       \
                                                                               \
    Result (*const push)(VecT *const, const T);                                \
    Result (*const insert)(VecT *const, const size_t, const T);                \
    Result (*const remove)(VecT *const, const size_t);                         \
    Result (*const pop)(VecT *const);                                          \
                                                                               \
    Result (*const clear)(VecT *const);                                        \
  }

/// @brief Inits an ampty vec.
/// @param self valid pointer
/// @return
///   `true` - success;
///   `false` - `malloc` failed
#define vec_vtbl_init(self) (self, VecT)->vtbl = &private_##VecT##_vtbl

/// @brief Inits an ampty vec.
/// @param self valid pointer
/// @param VecT vector type (for getting vtable)
/// @return `Result`
#define vec_init(self, VecT) vec_vtbl_init(self, VecT), (self)->vtbl->init(self)

/// @brief Inits a vec and populates it with values from `arr`.
/// @param self valid pointer
/// @param arr array to take values from
/// @param arr_len length of `arr`
/// @return `Result`
#define vec_init_from_arr(self, VecT, arr, arr_len)                            \
  vec_vtbl_init(self, VecT),                                                   \
    (self)->vtbl->init_from_arr((self), (arr), (arr_len))

/// @brief Inits a vec and populates it `n` times with `element` value from
/// `arr`.
/// @param self valid pointer
/// @param element value to populate with
/// @param n how many times `element` is repeated
/// @return `Result`
#define vec_init_filled(self, VecT, element, n)                                \
  vec_vtbl_init(self, VecT), (self)->vtbl->init_filled((self), (element), (n))

/// @brief Uninits a vec. Using other functions on an uninited vec can cause
/// undefined behavior.
/// @param self valid pointer
/// @return `void`
#define vec_uninit(self) (self)->vtbl->uninit(self)

/// @brief Get vec's len.
/// @param self valid pointer
/// @return vec's len
#define vec_len(self) (self)->len

/// @brief Get vec's capacity.
/// @param self valid pointer
/// @return vec's capacity
#define vec_cap(self) (self)->cap

/// @brief Get vec's internal buffer.
/// @param self valid pointer
/// @return pointer to vec's buffer
#define vec_buf(self) (self)->data

/// @brief Gets the element (with bounds checking).
/// @param self valid pointer
/// @param index any number
/// @return
///   element pointer - success;
///   `NULL` - bounds check failed
#define vec_at(self, index) (self)->vtbl->at(self, index)

/// @brief Gets the first element.
/// @param self valid pointer
/// @return
///   element pointer - success;
///   `NULL` – vec is empty
#define vec_first(self) (self)->vtbl->first(self)

/// @brief Gets the last element.
/// @param self valid pointer
/// @return
///   element pointer - success;
///   `NULL` – vec is empty
#define vec_last(self) (self)->vtbl->last(self)

/// @brief Pushes an element into a vec.
/// @param self valid pointer
/// @param element value to push
/// @return `Result`
#define vec_push(self, element) (self)->vtbl->push(self, element)

/// @brief Inserts an element into a vec.
/// @param self valid pointer
/// @param index any number
/// @param element value to insert
/// @return `Result`
#define vec_insert(self, index, element)                                       \
  (self)->vtbl->insert(self, index, element)

/// @brief Removes an element from a vec.
/// @param self valid pointer
/// @param index any number
/// @return `Result`
#define vec_remove(self, index) (self)->vtbl->remove(self, index)

/// @brief Pops an element from a vec.
/// @param self valid pointer
/// @return `Result`
#define vec_pop(self) (self)->vtbl->pop(self)

/// @brief Clears a vec.
/// @param self valid pointer
/// @return `Result` (technically, failure is impossable, but who really knows?)
#define vec_clear(self) (self)->vtbl->clear(self)

decl_vec(AnyVec, void *);

Result any_vec_init(AnyVec *const self, const size_t el_size);
void any_vec_uninit(AnyVec *const self);

Result any_vec_increment(AnyVec *const self);
Result any_vec_decrement(AnyVec *const self);

Result any_vec_reset(AnyVec *const self);

#endif
