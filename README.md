# vec

Typesafe vector data structure, written in plain C.


## Usage

To provide type safety, struct name and type (and optionally a function prefix) should be provided before inclusion of the header file:

```c
#define Self VecOfFloat // vector struct name
#define T    float // element type
#define PREFIX vec_of_float // you can also redefine the function prefix to follow your naming style
#include <vec.h> 
// `Self`, `T` and `PREFIX` have been undefined in `vec.h`
```

This setup must only be done a single time. If you need to use the same vec type in multiple header files, you'll need to create a separate header, excllusive for your vec and mark it with either include guards or `#pragma once`. This cannot be done in the library, because the same file `vec.h` is shared between multiple vector types.

After that setup you will be able to use the vec:

```c
// all functions that may fail, return `enum Result` representing the error that can occure
// you can use the `UNROLL` macro, which will return only if result is not equal to `Ok`
VecOfFloat vec;
UNROLL(vec_of_float_init(&vec));

UNROLL(vec_of_float_push(&vec, 0)); 
UNROLL(vec_of_float_push(&vec, 1)); 
UNROLL(vec_of_float_push(&vec, 2)); 

// if vec is empty, `start` and `end` functions will return `NULL`s, so loop will exit immediately
for(float *el_ptr = vec_of_float_start(&vec); el_ptr < vec_of_float_end(&vec); el_ptr++) 
    *el_ptr++;

vec_of_float_uninit(&vec); 
// watch out: using uninitialized vec may cause undefined behaviour
```

See [`test.c`](./test.c) for more examples.

> [!note]
> You may wonder, what if someone wants to change the vec's type, but don't want to rename a bunch of functions all througout the code? There is actually a pretty elegant solution. In languages, where vector is built in, it is usually used as a generic with a type parameter. This is defenetely not a bad thing, but sometimes can be a little bit messy, especially with longer or nested type names. Also this is not really a good suit for C.
>
So, my advice is naming vecs not based on the element type, but based on their purpose, for example:
>
> ```c
> #define Self VecOfConstCharPtr // bad
> #define Self VecOfEnvVar // good
> #define T char const *
> #include <vec.h>
> 
> #define Self VecOfConstCharPtr // not again!
> #define Self ExcludedHttpHeaders // you can even omit 'vec' entirely!
> #define T char const *
> #include <vec.h>
> ```
>
> When you get used to it, you'll see that this actually makes code much cleaner. As a nice bonus, library will protect you from inconsistencies, e.g. using `VecOfEnvVar_init` with an `ExcludedHttpHeaders` vec.


## Performance

At the moment, performance is slightly worse than the `std::vector` from C++, most likely due to a different memory resizing strategy. On the other hand, the resulting code size will be much smaller, as all functions are implemented only once, instead of generated for the each type. This make it suitable for more restricted enviroments (especially when custom allocators will be supported).


## ToDo
 - come up with better name;
 - add support for custom allocator;
 - add an ability for reserving vec capacit;
 - improve performance (possibly storing store end pointer and data end pointer, instead of `len` and `cap` will help).
