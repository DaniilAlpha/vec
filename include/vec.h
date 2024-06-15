#ifndef Self
#  define Self VecOfPtr
#  error "'Self' for the template is not defined!"
#endif

#ifndef T
#  define T void const *
#  error "'T' for the template is not defined!"
#endif

#include "vec_decl.h"

INTERNAL__VEC_DECL(Self, T);

void INTERNAL__METHOD(Self, init_vtbl)(Self *const self);

#undef Self
#undef T
