#ifndef VecT
# define VecT VecOfPtr
# error "'VecT' for the template is not defined!"
#endif

#ifndef ElT
# define ElT void const *
# error "'ElT' for the template is not defined!"
#endif

#include <stddef.h>

#include "any_vec.h"

private_decl_vec(VecT, ElT);

void private_vec_fn1(VecT, init_vtbl)(VecT *const self);

#undef VecT
#undef ElT
