#ifndef VEC_DECL_H
#define VEC_DECL_H

#include <stddef.h>

#include <result.h>

#define private_vtbl(Self)     Self##Vtbl
#define private_vec_vtbl(Self) private_vtbl(Self)

#define private_method_(type, name) (private_##type##_##name)
#define private_method(type, name)  private_method_(type, name)

#define private_vec_decl(Self, T)                                              \
    typedef struct private_vec_vtbl(Self) private_vec_vtbl(Self);              \
    typedef struct Self {                                                      \
        private_vec_vtbl(Self) const *vtbl;                                    \
        size_t len, cap, el_size;                                              \
        T *data;                                                               \
    } Self;                                                                    \
    struct private_vec_vtbl(Self) {                                            \
        Result (*const init)(Self *const);                                     \
        Result (*const init_from_arr)(                                         \
            Self *const,                                                       \
            T const *const,                                                    \
            size_t const                                                       \
        );                                                                     \
        Result (*const init_filled)(Self *const, T const, size_t const);       \
        void (*const uninit)(Self *const);                                     \
                                                                               \
        T *(*const at)(Self const *const, size_t const);                       \
                                                                               \
        Result (*const push)(Self *const, T const);                            \
        Result (*const insert)(Self *const, size_t const, T const);            \
        Result (*const remove)(Self *const, size_t const);                     \
        Result (*const pop)(Self *const);                                      \
                                                                               \
        Result (*const clear)(Self *const);                                    \
    }

#define private_vec_vtbl_init(self, Self) private_method(Self, init_vtbl)(self)

/// @brief Inits an ampty vec.
/// @param self valid pointer
/// @param Self vector type (for getting vtable)
/// @return `Result`
#define vec_init(self, Self)                                                   \
    (private_vec_vtbl_init((self), Self), (self)->vtbl->init(self))
/// @brief Uninits a vec. Using other functions on an uninited vec can cause
/// undefined behavior.
/// @param self valid pointer
/// @return `void`
#define vec_uninit(self) ((self)->vtbl->uninit(self))

/// @brief Inits a vec and populates it with values from `arr`.
/// @param self valid pointer
/// @param Self vector type (for getting vtable)
/// @param arr array to take values from
/// @param len length of `arr`
/// @return `Result`
#define vec_init_from_arr(self, Self, arr, len)                                \
    (private_vec_vtbl_init((self), Self),                                      \
     (self)->vtbl->init_from_arr((self), (arr), (len)))

/// @brief Inits a vec and populates it `n` times with `element` value from
/// `arr`.
/// @param self valid pointer
/// @param Self vector type (for getting vtable)
/// @param element value to populate with
/// @param n how many times `element` is repeated
/// @return `Result`
#define vec_init_filled(self, Self, element, n)                                \
    (private_vec_vtbl_init((self), Self),                                      \
     (self)->vtbl->init_filled((self), (element), (n)))

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
#define vec_first(self) ((self)->len == 0 ? NULL : (self)->data)
/// @brief Gets the last element.
/// @param self valid pointer
/// @return
///   element pointer - success;
///   `NULL` – vec is empty
#define vec_last(self)                                                         \
    ((self)->len == 0 ? NULL : (self)->data + (self)->len - 1)

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

#endif
