# Vec

Plain-C type-safe vector data strucutre.

The main goal was type-safety. It was achieved at the cost of a little performance overhead due to vtable usage, but still comparable to std's one.

## Usage

To allow type safety, the API is separated into two files. When you need to use the vector type, use: 
```c
#define Self VecOfFloat // name of the vec type (vector struct is declared with it)
#define T    float // element type
#include <vec.h> // vector declaration that uses previously-defined `Self` and `T`
// `Self` and `T` got undefined by `vec.h`
```

And **only once** you include the implementation file:
```c
// this implementation will be used for all vectors of type defined by `Self`
#define Self VecOfFloat
#define T    float
#include <vec_impl.h> 
```

API looks like this: 
```c
VecOfFloat vec;
// passing the right type here is crucial, as this is the step of determining the vtable to use on the vec
if(vec_init(&vec, VecOfFloat) != 0) exit(1);

// all functions that may fail, return the `enum Result` representing the error occured
// you may use the `unroll` macro that will return result if it is not `Ok`, or just comapre it with `Ok` (or `0`)

if(vec_push(&vec, 0) != 0 || 
    vec_push(&vec, 1) != 0 || 
    vec_push(&vec, 2) != 0) 
    exit(1);

for(float *el = vec_first(&vec); el <= vec_last(&vec); el++ ) 
    *el++;

vec_uninit(&vec); // should be inited again after this call
```
`vec_init`, `vec_push`, etc. are the function-like macros. This makes it easy to swap vector type to another one and not needing to rename a whole bunch of functions. I prefer to name them in lowercase, as they are (mostly) only shortcuts for acessing vtable. For example `vec_push(self)` expands to `self->vtbl->push(self)`. 

For more examples, see [`test.c`](./test.c).

## TODO 
 - support custom allocators
 - add logic to reserve vec capacity