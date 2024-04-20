#ifndef Self
#  define Self VecOfPtr
#  error "'Self' for the template is not defined!"
#endif

#ifndef T
#  define T void const *
#  error "'T' for the template is not defined!"
#endif

#include "vec_decl.h"

private_vec_decl(Self, T);

void private_method(Self, init_vtbl)(Self *const self);

#undef Self
#undef T
