# Vec

Plain C typesafe vector data structure.

## Usage

To provide type safety, the API is split into two files. If you need to use the vector type, use:
```c
#define Self VecOfFloat // name of the vec type (vector struct is declared with it)
#define T    float // element type
#include <vec.h> // vector declaration using previously defined `Self` and `T`
// `Self` and `T` have been undefined by `vec.h`

```

And **only once** include the implementation file:
```c
// this implementation will be used for all vectors of type defined by `Self`
#define Self VecOfFloat
#define T    float
#include <vec_impl.h> 
```

The API looks like this: 
```c
VecOfFloat vec;
// passing the correct type here is crucial, as this is the step that determines the vtable to be used on the vec
if(vec_init(&vec, VecOfFloat) != Ok) return 1;

// all functions that may fail, return the `enum Result` representing the error that occured
// you can use the `UNROLL` macro, which will return the result if it is not `Ok`, or just comapre it to `Ok`

UNROLL(vec_push(&vec, 0)); 
UNROLL(vec_push(&vec, 1)); 
UNROLL(vec_push(&vec, 2)); 

// this should be done carefully as `vec_first` and `vec_last` return `NULL` if the vec is empty
for(float *el = vec_first(&vec); el <= vec_last(&vec); el++ ) 
    *el++;

vec_uninit(&vec); // should be reinited after this call
```
> `vec_init`, `vec_push`, etc. are the function-like macros. This makes it easy to switch from one vector type to another without having to rename a bunch of functions. I prefer to name them in lowercase because they are (mostly) just shortcuts for accessing vtable. For example `vec_push(self)` expands to `self->vtbl->push(self)`. 

See [`test.c`](./test.c) for more examples.

## Performance
The main goal of vec is type safety. This has been achieved at the cost of a small performance overhead over std::vector. (Possibly) due to the use of vtables, vec is slightly slower. However, when reallocating memory or moving large numbers of elements, the difference is negligible. On the other hand, it should use less disk space, since most of the logic is shared via non-typesafe functions covered by the typesafe API.

## TODO
 - come up with better library name
 - support custom allocators
 - add logic to reserve vec capacity
 - make vec store `data_end`, `space_end` instead of `len` and `cap` to increase performance
